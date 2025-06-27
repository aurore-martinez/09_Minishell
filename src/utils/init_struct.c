/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aumartin <aumartin@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 16:25:25 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/27 19:02:58 by aumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_shell	*get_shell(void)
{
	static t_shell	shell;

	return (&shell);
}

/* tableau d'objets pour tableau de func KEY VALEUR voir ds tests funcptr.c */
// NULL de fin pour boucle cond while (builtins[i].name)
static void	get_builtins(t_builtin	*builtins)
{
	builtins[0] = (t_builtin){"pwd", ft_pwd};
	builtins[1] = (t_builtin){"env", ft_env};
	builtins[2] = (t_builtin){"echo", ft_echo};
	builtins[3] = (t_builtin){"cd", ft_cd};
	builtins[4] = (t_builtin){"exit", ft_exit};
	builtins[5] = (t_builtin){"export", ft_export};
	builtins[6] = (t_builtin){"unset", ft_unset};
	builtins[7] = (t_builtin){NULL, NULL};
}

void	init_shell(void)
{
	t_shell		*shell;

	shell = get_shell();
	ft_bzero(shell, sizeof(t_shell));
	get_builtins(shell->builtins);
}

// t_token	*init_token(t_token	*token)
// {
// 	token = malloc(sizeof(t_token));
// 	if (!token)
// 		return ;
// 	token->type = -1;
// 	token->node_num = 0;
// 	token->value = NULL;
// 	token->prev = NULL;
// 	token->next = NULL;
// 	return (token);
// }
