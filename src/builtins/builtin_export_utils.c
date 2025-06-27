/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 18:45:30 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/27 18:56:56 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_env_export(t_env *env)
{
	while (env)
	{
		printf("export %s=", env->key);
		if (env->value)
			printf("\"%s\"", env->value);
		printf("\n");
		env = env->next;
	}
}

int	is_valid_identifier(char *arg)
{
	if (!arg || !arg[0] || ft_isdigit(arg[0]) || arg[0] == '=')
	{
		write(2, "minishell: export: `", 20);
		write(2, arg, ft_strlen(arg));
		write(2, "': not a valid identifier\n", 26);
		return (false);
	}
	return (true);
}
