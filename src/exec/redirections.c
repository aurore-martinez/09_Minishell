/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:20:15 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/27 22:06:41 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* applique les dup2 pour rediriger in et out
changement condi car heredoc en pipe, pipe_fd[0] == 0
donc je dois faire dup2() si fd_in != STDIN_FILENO
*/
void	apply_dup_redirections(t_cmd *cmd)
{
	if (cmd->fd_in != STDIN_FILENO)
	{
		if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
			perror_free_gc("dup2 (input)");
	}
	if (cmd->fd_out != STDOUT_FILENO)
	{
		if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
			perror_free_gc("dup2 (output)");
	}
}

/* ferme les fd ouverts pour les redir
fd_in et fd_out mis -1 => permet de savoir etat aps qu'ils sont fermes
et donc eviter de les refermer par erreur.
*/
void	close_redirections(t_cmd *cmd)
{
	if (cmd->fd_in > 2)
	{
		close(cmd->fd_in);
		cmd->fd_in = -1;
	}
	if (cmd->fd_out > 2)
	{
		close(cmd->fd_out);
		cmd->fd_out = -1;
	}
}

int	apply_redirections(t_cmd *cmd)
{
	t_redir	*redir;

	if (!cmd || !cmd->redir)
		return (0);
	redir = cmd->redir;
	if (check_redirections_consistency(cmd) == -1)
		return (-1);
	apply_dup_redirections(cmd);
	close_redirections(cmd);
	return (0);
}
