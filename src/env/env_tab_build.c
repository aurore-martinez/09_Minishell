/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_tab_build.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 13:16:06 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/27 22:17:46 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* compter les elements de l'env valides (avec '=') */
static int	count_env_tab(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		if (env->equal == true)
			count++;
		env = env->next;
	}
	return (count);
}

/* transformer un element env en string "KEY=VALUE" pour remplir le tab
 * 🚩 Pourquoi GC_CMD ici ?
 * → Parce que c’est utilisé uniquement pour execve (cmd exec)
 * → La mémoire sera libérée après l'exécution de la commande.
 */
static char	*fill_env_tab(t_env *entry)
{
	char	*line;
	size_t	len;

	len = ft_strlen(entry->key) + 1 + ft_strlen(entry->value) + 1;
	line = gc_mem(GC_ALLOC, len, NULL, GC_CMD);
	if (!line)
		perror_free_gc("minishell: fill_env_tab: allocation failed\n");
	ft_strlcpy(line, entry->key, len);
	ft_strlcat(line, "=", len);
	ft_strlcat(line, entry->value, len);
	return (line);
}

/* construction du char ** depuis t_env pour execve */
char	**env_to_env_tab_for_execve(t_env *env)
{
	int		i;
	int		count;
	char	**env_tab;

	count = count_env_tab(env);
	env_tab = gc_mem(GC_ALLOC, sizeof(char *) * (count + 1), NULL, GC_CMD);
	if (!env_tab)
		error_exit("minishell: env_to_env_tab_for_execve: alloc failed");
	i = 0;
	while (env)
	{
		if (env->equal == true)
			env_tab[i++] = fill_env_tab(env);
		env = env->next;
	}
	env_tab[i] = NULL;
	return (env_tab);
}
