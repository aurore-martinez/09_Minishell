/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 11:37:06 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/27 20:15:52 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Afficher toutes les variables d’environnement actuelles

en bash :
env
Sans args : affiche l’environnement courant (copie exportée).

env VAR1=value1 VAR2=value2 command args
Avec VAR=value + commande : crée un environnement temporaire pour cette commande.
Ne montre que les variables exportées.

Les variables créées via VAR=value sans export ne sont pas visibles dans env.

=> 	env affiche les variables de t_env où equal == true + exit_status */

#include "../../include/minishell.h"

static int	handle_env_options(t_shell *shell, t_cmd *cmd)
{
	if (!cmd->args[1])
		return (0);
	if (is_directory(cmd->args[1]))
	{
		ft_putstr_fd("env: '", STDERR_FILENO);
		ft_putstr_fd(cmd->args[1], STDERR_FILENO);
		ft_putstr_fd("': Is a directory\n", STDERR_FILENO);
		shell->exit_status = 126;
		return (126);
	}
	if (cmd->args[1][0] == '\0')
	{
		ft_putstr_fd("env: '", STDERR_FILENO);
		ft_putstr_fd("': command not found\n", STDERR_FILENO);
		shell->exit_status = 127;
		return (127);
	}
	ft_putstr_fd("env: '", STDERR_FILENO);
	ft_putstr_fd(cmd->args[1], STDERR_FILENO);
	ft_putstr_fd("': No such file or directory\n", STDERR_FILENO);
	shell->exit_status = 127;
	return (127);
}

int	ft_env(t_shell *shell, t_cmd *cmd, int fd)
{
	t_env	*curr;
	int		result;

	(void) cmd;
	result = handle_env_options(shell, cmd);
	if (result != 0)
		return (result);
	curr = shell->env;
	while (curr)
	{
		if (curr->value)
		{
			ft_putstr_fd(curr->key, fd);
			ft_putstr_fd("=", fd);
			ft_putstr_fd(curr->value, fd);
			ft_putstr_fd("\n", fd);
		}
		else if (curr->equal && !curr->value)
		{
			ft_putstr_fd(curr->key, fd);
			ft_putstr_fd("=\n", fd);
		}
		curr = curr->next;
	}
	return (0);
}
