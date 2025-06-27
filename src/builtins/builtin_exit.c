/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 11:41:41 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/26 17:47:36 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_exit(t_shell *shell, t_cmd *cmd, int fd)
{
	int	exit_code;

	(void)fd;
	exit_code = shell->exit_status;
	if (!cmd->prev && !cmd->next)
		ft_putstr_fd("exit\n", STDERR_FILENO);
	if (!cmd->args[1])
	{
		gc_mem(GC_FREE_ALL, 0, NULL, GC_NONE);
		exit(exit_code);
	}
	if (!is_numeric(cmd->args[1]))
	{
		ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
		gc_mem(GC_FREE_ALL, 0, NULL, GC_NONE);
		exit(2);
	}
	if (cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	exit_code = ft_atoi(cmd->args[1]) % 256;
	gc_mem(GC_FREE_ALL, 0, NULL, GC_NONE);
	exit(exit_code);
}
