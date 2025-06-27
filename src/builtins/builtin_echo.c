/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 12:12:48 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/10 15:31:37 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Affiche tous les args + les sépare par des espaces + ajoute un \n à la fin.

en bash :
echo				-> (juste une ligne vide)
echo hello			-> hello
echo hello world	-> hello world

Parcours les arguments args[1] jusqu’à la fin.
Affiches séparés par des espaces.
Termines par un \n.
Mets à jour shell->exit_status = 0. */

// A FaIRE : Gérer option -n (sans retour à la ligne)
// bash accepte plusieurs n collés : echo -nnnn hello -> donne hello sans \n.

#include "../../include/minishell.h"

t_bool	is_option_n(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (false);
	i = 2;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

int	ft_echo(t_shell *shell, t_cmd *cmd, int fd)
{
	int		i;
	t_bool	no_newline;
	char	**args;

	if (!shell || !cmd || !cmd->args)
		return (1);
	args = cmd->args;
	i = 1;
	no_newline = false;
	while (args[i] && is_option_n(args[i]))
	{
		no_newline = true;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], fd);
		if (args[i + 1])
			ft_putstr_fd(" ", fd);
		i++;
	}
	if (!no_newline)
		ft_putstr_fd("\n", fd);
	shell->exit_status = 0;
	return (0);
}
