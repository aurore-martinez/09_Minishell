/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_builtins.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/04 08:57:44 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/26 20:23:03 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_builtin(t_shell *shell, t_cmd *cmd, int fd)
{
	int			i;
	char		**args;
	t_builtin	*builtins;

	args = cmd->args;
	builtins = shell->builtins;
	i = 0;
	if (!args || !args[0])
		return (0);
	while (builtins[i].name)
	{
		if (ft_strcmp(args[0], builtins[i].name) == 0)
		{
			if (ft_strcmp(args[0], "exit") == 0)
				restore_std(&shell->std_backup);
			return (builtins[i].func(shell, cmd, fd));
		}
		i++;
	}
	return (0);
}

/* a intreger au parsing (dès que tu as le t_cmd)
is_builtin() ➔ remplir cmd->is_builtin */
t_bool	is_builtin(t_shell *shell, char *cmd_name)
{
	int			i;
	t_builtin	*builtins;

	i = 0;
	builtins = shell->builtins;
	if (!cmd_name)
		return (false);
	while (builtins[i].name)
	{
		if (ft_strcmp(cmd_name, builtins[i].name) == 0)
			return (true);
		i++;
	}
	return (false);
}
