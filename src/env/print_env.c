/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 09:46:11 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/09 18:19:58 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_envp(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
		printf("%s\n", envp[i++]);
}

void	print_env_line(char *line)
{
	char	*equal;

	equal = ft_strchr(line, '=');
	if (!equal)
		return ;
	write(1, "KEY: ", 5);
	write(1, line, equal - line);
	write(1, "\nVALUE: ", 8);
	write(1, equal + 1, ft_strlen(equal + 1));
	write(1, "\n\n", 2);
}

void	print_env(t_env *env)
{
	while (env)
	{
		printf("key: %s | equal: %d | value: ", env->key, env->equal);
		if (env->value)
			printf("%s", env->value);
		else
			printf("(null)");
		printf("\n");
		env = env->next;
	}
}

void	print_env_tab(char **env_tab)
{
	int	i;

	if (!env_tab)
	{
		ft_printf("env_tab est NULL\n");
		return ;
	}
	i = 0;
	while (env_tab[i])
	{
		ft_printf("env_tab[%d] = %s\n", i, env_tab[i]);
		i++;
	}
}
