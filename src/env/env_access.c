/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_access.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 10:22:26 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/27 18:16:19 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* mettre a jour une variable d'environnement dans t_env */
void	update_env_value(t_env *env, const char *key, const char *new_value)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
		{
			if (env->value)
				gc_mem(GC_FREE_ONE, 0, env->value, GC_ENV);
			env->equal = true;
			env->value = gc_strdup(new_value, GC_ENV);
			return ;
		}
		env = env->next;
	}
}

/* recup la valeur depuis la clé */
char	*get_env_value(t_env *env, const char *key, t_gc_type type)
{
	char	*res;

	res = NULL;
	if (!key)
		return (NULL);
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0 && env->equal == true)
		{
			res = env->value;
			if (!res)
				return (NULL);
			return (res);
		}
		env = env->next;
	}
	res = gc_strdup("", type);
	if (!res)
		return (NULL);
	return (res);
}
