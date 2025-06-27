/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:51:31 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/27 20:06:09 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handle_ctrl_c(t_shell *shell)
{
	if (g_sig == SIGINT)
	{
		shell->exit_status = 130;
		g_sig = 0;
	}
}

static void	handle_ctrl_d(t_shell *shell)
{
	int	res;

	res = shell->exit_status;
	gc_mem(GC_FREE_ALL, 0, NULL, GC_NONE);
	rl_clear_history();
	ft_putstr_fd("exit\n", 2);
	exit(res);
}

static int	process_prompt_line(char *prompt, t_cmd **cmd_list, t_shell *shell)
{
	add_history(prompt);
	if (!parsing(prompt, cmd_list, shell))
	{
		gc_mem(GC_FREE_ALL, 0, NULL, GC_TKN);
		gc_mem(GC_FREE_ALL, 0, NULL, GC_CMD);
		*cmd_list = NULL;
		return (false);
	}
	gc_mem(GC_FREE_ALL, 0, NULL, GC_TKN);
	set_signals_exec();
	exec_dispatcher(*cmd_list, shell);
	handle_ctrl_c(shell);
	set_signals_interactive();
	gc_mem(GC_FREE_ALL, 0, NULL, GC_CMD);
	return (true);
}

void	ft_prompt(t_shell *shell)
{
	char		*prompt;
	t_cmd		*cmd_list;

	init_signals();
	while (1)
	{
		cmd_list = NULL;
		prompt = readline(MAGENTA "minishell:" RESET);
		handle_ctrl_c(shell);
		if (!prompt)
			handle_ctrl_d(shell);
		if (*prompt)
		{
			if (!process_prompt_line(prompt, &cmd_list, shell))
			{
				free(prompt);
				continue ;
			}
		}
		free(prompt);
	}
}
