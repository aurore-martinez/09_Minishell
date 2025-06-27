/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expnd_lst_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:32:48 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/27 22:06:41 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*ft_lstnewtoken_xpnd(void)
{
	t_token	*new_token;

	new_token = gc_mem(GC_ALLOC, sizeof(t_token), NULL, GC_TKN);
	if (!new_token)
		perror_free_gc("ft_lstnewtoken_xpnd alloc failure.\n");
	new_token->token_type = -1;
	new_token->token_raw = NULL;
	new_token->token_value = NULL;
	new_token->prev = NULL;
	new_token->next = NULL;
	return (new_token);
}

t_xpnd	*ft_lstlast_xpnd(t_xpnd *xpnd)
{
	while (xpnd && xpnd->next)
		xpnd = xpnd->next;
	return (xpnd);
}

void	ft_lstadd_back_xpnd(t_xpnd **xpnd, t_xpnd *new)
{
	t_xpnd	*temp;

	if (xpnd == NULL || new == NULL)
		return ;
	if (*xpnd == NULL)
	{
		*xpnd = new;
		new->prev = NULL;
	}
	else
	{
		temp = ft_lstlast_xpnd(*xpnd);
		new->prev = temp;
		temp->next = new;
	}
}

t_xpnd	*ft_lstnewxpnd(void)
{
	t_xpnd	*new_xpnd;

	new_xpnd = gc_mem(GC_ALLOC, sizeof(t_xpnd), NULL, GC_TKN);
	if (!new_xpnd)
		perror_free_gc("ft_lstnewxpnd alloc failure.\n");
	new_xpnd->substr = NULL;
	new_xpnd->in_double = false;
	new_xpnd->in_single = false;
	new_xpnd->xpnd_check = -1;
	new_xpnd->str_to_join = NULL;
	new_xpnd->prev = NULL;
	new_xpnd->next = NULL;
	return (new_xpnd);
}

void	free_t_xpnd_list(t_xpnd *xpnd_quotes_list)
{
	t_xpnd	*xpnd_curr;
	t_xpnd	*xpnd_next;

	if (!xpnd_quotes_list)
		return ;
	xpnd_curr = xpnd_quotes_list;
	while (xpnd_curr && xpnd_curr->prev)
		xpnd_curr = xpnd_curr->prev;
	while (xpnd_curr)
	{
		xpnd_next = xpnd_curr->next;
		if (xpnd_curr->substr)
			free(xpnd_curr->substr);
		if (xpnd_curr->str_to_join)
			free(xpnd_curr->str_to_join);
		free(xpnd_curr);
		xpnd_curr = xpnd_next;
	}
}
