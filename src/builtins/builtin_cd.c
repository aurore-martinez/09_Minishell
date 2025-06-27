/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 14:17:01 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/26 21:42:37 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Implémenter la commande builtin `cd`
// Mettre à jour PWD et OLDPWD dans l'environnement
// Gérer les erreurs et messages appropriés

#include "../../include/minishell.h"

/* gere les cas speciaux comme '~', '-', ou autres erreurs */
static int	handle_special_cases(t_cmd *cmd)
{
	if (!cmd->args[0])
	{
		ft_putstr_fd("minishell: cd: missing path\n", STDERR_FILENO);
		ft_putstr_fd("Usage: cd <path> relative or absolute\n", STDERR_FILENO);
		return (1);
	}
	if (cmd->args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	if (ft_strncmp(cmd->args[0], "~", 1) == 0)
	{
		ft_putstr_fd("minishell: cd: '~' not supported \n", STDERR_FILENO);
		ft_putstr_fd("Usage: cd: $HOME or cd <path>\n", STDERR_FILENO);
		return (1);
	}
	if (ft_strncmp(cmd->args[0], "-", 1) == 0)
	{
		ft_putstr_fd("minishell: cd: '-' not supported \n", STDERR_FILENO);
		ft_putstr_fd("Usage: cd <path> relative or absolute\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

/* si repertoire courant a ette supprime
on recup l'ancien ou par defaut aller à /   */
static int	handle_deleted_cwd(t_shell *shell)
{
	char	*cwd;
	char	*oldpwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
		return (free(cwd), 0);
	ft_putstr_fd("minishell: cd: current directory deleted\n", STDERR_FILENO);
	oldpwd = get_env_value(shell->env, "OLDPWD", GC_ENV);
	if (oldpwd && chdir(oldpwd) == 0)
	{
		ft_putstr_fd("minishell: cd: restored to OLDPWD\n", STDERR_FILENO);
		update_env_value(shell->env, "PWD", gc_strdup(oldpwd, GC_ENV));
		return (0);
	}
	if (chdir("/") == 0)
	{
		ft_putstr_fd("minishell: cd: restored to /\n", STDERR_FILENO);
		update_env_value(shell->env, "PWD", gc_strdup("/", GC_ENV));
		return (0);
	}
	return (1);
}

/* handle_special_cases() traite cd seul, cd ~, cd -.
ft_cd() applique getcwd
chdir, met à jour OLDPWD et PWD en utilisant gc_strdup
free() pour getcwd() qui est un malloc. */

int	ft_cd(t_shell *shell, t_cmd *cmd, int fd)
{
	char	*old_pwd;
	char	*new_pwd;

	(void)fd;
	if (handle_deleted_cwd(shell) != 0)
		return (shell->exit_status = 1, 1);
	if (handle_special_cases(cmd))
		return (shell->exit_status = 1, 1);
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
		return (perror("minishell: cd: getcwd"), 1);
	if (chdir(cmd->args[1]) != 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		free(old_pwd);
		return (perror(cmd->args[0]), 1);
	}
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
		return (perror("minishell: cd: getcwd"), free(old_pwd), 1);
	update_env_value(shell->env, "OLDPWD", gc_strdup(old_pwd, GC_ENV));
	update_env_value(shell->env, "PWD", gc_strdup(new_pwd, GC_ENV));
	free(old_pwd);
	free(new_pwd);
	return (0);
}
