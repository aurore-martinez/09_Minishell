/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 16:57:36 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/27 16:37:05 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* save les descripteurs originaux de STDIN/STDOUT */
void	save_std(t_std_backup *backup)
{
	backup->orig_stdin = dup(STDIN_FILENO);
	backup->orig_stdout = dup(STDOUT_FILENO);
	backup->orig_stderr = dup(STDERR_FILENO);
}

/* restaure les descripteurs STDIN/STDOUT depuis la sauvegarde */
void	restore_std(t_std_backup *backup)
{
	if (backup->orig_stdin >= 3)
	{
		dup2(backup->orig_stdin, STDIN_FILENO);
		close(backup->orig_stdin);
		backup->orig_stdin = 0;
	}
	if (backup->orig_stdout >= 3)
	{
		dup2(backup->orig_stdout, STDOUT_FILENO);
		close(backup->orig_stdout);
		backup->orig_stdout = 0;
	}
	if (backup->orig_stderr >= 3)
	{
		dup2(backup->orig_stderr, STDERR_FILENO);
		close(backup->orig_stderr);
		backup->orig_stderr = 0;
	}
}

void	single_cmd_childhood(t_cmd *cmd, t_shell *shell)
{
	int		exit_status;
	char	*path;

	exit_status = 0;
	path = NULL;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (apply_redirections(cmd) == -1)
	{
		gc_mem(GC_FREE_ALL, 0, NULL, GC_NONE);
		exit (1);
	}
	if (is_valid_command(cmd, shell, &exit_status, &path))
		execve(path, cmd->args, env_to_env_tab_for_execve(shell->env));
	gc_mem(GC_FREE_ALL, 0, NULL, GC_NONE);
	exit(exit_status);
}

/* single commande sans pipe */
void	exec_single_cmd(t_cmd *cmd, t_shell *shell)
{
	pid_t			pid;

	if (cmd->is_builtin)
	{
		save_std(&shell->std_backup);
		if (apply_redirections(cmd) == -1)
		{
			shell->exit_status = 1;
			restore_std(&shell->std_backup);
			return ;
		}
		shell->exit_status = handle_builtin(shell, cmd, STDOUT_FILENO);
		restore_std(&shell->std_backup);
	}
	else
	{
		pid = fork();
		if (pid == 0)
			single_cmd_childhood(cmd, shell);
		wait_for_children(cmd, shell);
	}
}

/* waitpid du ou des enfants */
void	wait_for_children(t_cmd *cmds, t_shell *shell)
{
	t_cmd	*current;
	int		status;

	status = 0;
	current = cmds;
	while (current)
	{
		waitpid(current->pid, &status, 0);
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->exit_status = 128 + WTERMSIG(status);
		current = current->next;
	}
	if (shell->exit_status == 130)
		write(2, "\n", 1);
	else if (shell->exit_status == 131)
		write(2, "Quit (core dumped)\n", 19);
}
