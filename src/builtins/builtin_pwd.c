/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 11:52:10 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/27 21:31:15 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Afficher le chemin absolu courant

en bash :
pwd : Affiche le chemin absolu du répertoire courant.

bash utilise la variable $PWD, mais la vérifie avec getcwd() si besoin.

🧱 À implémenter
Utilise getcwd() directement.

Gère les erreurs si getcwd() échoue (ex : dossier supprimé). */

// pas gerer pour les redirections ???

#include "../../include/minishell.h"

int	ft_pwd(t_shell *shell, t_cmd *cmd, int fd)
{
	char	buf[PATH_MAX];

	(void) cmd;
	(void) shell;
	if (getcwd(buf, PATH_MAX))
	{
		ft_putstr_fd(buf, fd);
		ft_putstr_fd("\n", fd);
		shell->exit_status = 0;
		return (0);
	}
	else
	{
		perror("minishell : pwd");
		shell->exit_status = 1;
		return (1);
	}
}
