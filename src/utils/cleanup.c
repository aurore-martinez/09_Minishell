/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 07:54:42 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/26 20:15:27 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	cleanup_heredocs(t_cmd *cmd_list)
{
	t_cmd	*cmd;
	t_redir	*redir;

	cmd = cmd_list;
	while (cmd)
	{
		redir = cmd->redir;
		while (redir)
		{
			if (redir->type == TKN_HEREDOC && redir->file)
				unlink(redir->file);
			redir = redir->next;
		}
		cmd = cmd->next;
	}
}

void	free_and_cleanup_heredocs(t_cmd *cmd_list)
{
	cleanup_heredocs(cmd_list);
	gc_mem(GC_FREE_ALL, 0, NULL, GC_TMP);
}
