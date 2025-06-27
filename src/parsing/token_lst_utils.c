/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lst_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 17:40:28 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/27 22:06:41 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_token(t_token	*token_list)
{
	t_token	*temp;
	int		i;

	i = 0;
	if (!token_list)
		return ;
	temp = token_list;
	while (temp->prev)
	{
		temp = temp->prev;
	}
	while (temp)
	{
		printf("token[%d]: `%s` ; token_raw: `%s` ",
			i, temp->token_value, temp->token_raw);
		printf("; token_type: %d\n", temp->token_type);
		i++;
		temp = temp->next;
	}
}

t_token	*ft_lstnewtoken(char *prompt, int n, t_token_type token_type)
{
	t_token	*new_token;

	new_token = gc_mem(GC_ALLOC, sizeof(t_token), NULL, GC_TKN);
	if (!new_token)
		perror_free_gc("ft_lstnewtoken malloc failure\n");
	new_token->token_type = token_type;
	if (token_type != TKN_WORD)
		new_token->token_value = gc_strndup(prompt, n, GC_TKN);
	else
		new_token->token_value = gc_strndup_noquotes(prompt, n, GC_TKN);
	new_token->token_raw = gc_strndup(prompt, n, GC_TKN);
	new_token->prev = NULL;
	new_token->next = NULL;
	return (new_token);
}

t_token	*ft_lstlast_token(t_token *token)
{
	while (token && token->next)
		token = token->next;
	return (token);
}

void	ft_lstadd_back_token(t_token **token, t_token *new)
{
	t_token	*temp;

	if (token == NULL || new == NULL)
		return ;
	if (*token == NULL)
		*token = new;
	else
	{
		temp = ft_lstlast_token(*token);
		new->prev = temp;
		temp->next = new;
	}
}
