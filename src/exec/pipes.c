/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 14:56:38 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/27 22:06:41 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	close_all_pipes(t_cmd *command)
{
	t_cmd	*tmp;

	tmp = command;
	while (tmp->prev)
	{
		tmp = tmp->prev;
	}
	while (tmp->next)
	{
		close(tmp->pipe[0]);
		close(tmp->pipe[1]);
		tmp = tmp->next;
	}
}

static int	create_pipes(t_cmd *cmd)
{
	t_cmd	*command;

	command = cmd;
	while (command->next)
	{
		if (pipe(command->pipe) == -1)
		{
			close_all_pipes(cmd);
			return (-1);
		}
		command = command->next;
	}
	return (0);
}

/*
- Premier enfant** (`i == 0`) :
Ne fait **pas** de `dup2` pour `STDIN` (il garde l'entrée standard du shell).
Fait un `dup2` pour `STDOUT` vers le pipe de droite (`pipes[0][1]`).

- Dernier enfant** (`i == cmd_count - 1`) :
Fait un `dup2` pour `STDIN` depuis le pipe de gauche (`cmd->prev->pipes[0]`).
Ne fait **pas** de `dup2` pour `STDOUT` (il garde la sortie standard du shell).

- Enfants du milieu** (`0 < i < cmd_count - 1`) :
Fait un `dup2` pour `STDIN` depuis le pipe de gauche (`pipes[i-1][0]`).
Fait un `dup2` pour `STDOUT` vers le pipe de droite (`pipes[i][1]`).
*/

static void	apply_dup_pipeline(t_cmd *cmd)
{
	if (cmd->prev && cmd->next)
	{
		if (dup2(cmd->prev->pipe[0], STDIN_FILENO) == -1
			|| dup2(cmd->pipe[1], STDOUT_FILENO) == -1)
			perror_free_gc("dup2 failed (middle cmd)");
	}
	else if (cmd->next == NULL)
	{
		if (dup2(cmd->prev->pipe[0], STDIN_FILENO) == -1)
			perror_free_gc("dup2 failed (last cmd)");
	}
	else if (cmd->prev == NULL)
	{
		if (dup2(cmd->pipe[1], STDOUT_FILENO) == -1)
			perror_free_gc("dup2 failed (first cmd)");
	}
}

void	pipeline_childhood(t_cmd *cmd, t_shell *shell)
{
	int		exit_status;
	char	*path;

	exit_status = 0;
	path = NULL;
	apply_dup_pipeline(cmd);
	close_all_pipes(cmd);
	if (apply_redirections(cmd) == -1)
	{
		gc_mem(GC_FREE_ALL, 0, NULL, GC_NONE);
		exit (1);
	}
	if (!cmd->is_builtin && is_valid_command(cmd, shell, &exit_status, &path))
	{
		execve(path, cmd->args, env_to_env_tab_for_execve(shell->env));
		perror_free_gc("execve failed");
	}
	else if (cmd->is_builtin)
	{
		exit_status = handle_builtin(shell, cmd, STDOUT_FILENO);
		gc_mem(GC_FREE_ALL, 0, NULL, GC_NONE);
		exit (exit_status);
	}
	gc_mem(GC_FREE_ALL, 0, NULL, GC_NONE);
	exit(exit_status);
}

void	exec_pipeline(t_cmd *cmd_list, t_shell *shell)
{
	t_cmd	*cmd_curr;

	cmd_curr = cmd_list;
	if (create_pipes(cmd_list) == -1)
		perror_free_gc("create_pipes failed");
	while (cmd_curr)
	{
		cmd_curr->pid = fork();
		if (cmd_curr->pid < 0)
		{
			shell->exit_status = 1;
			perror_free_gc("fork failed");
		}
		if (cmd_curr->pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			pipeline_childhood(cmd_curr, shell);
		}
		cmd_curr = cmd_curr->next;
	}
	close_all_pipes(cmd_list);
	wait_for_children(cmd_list, shell);
	free_and_cleanup_heredocs(cmd_list);
}
