/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expnd_post_sgmt.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:32:56 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/27 22:10:18 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_empty_xpnd_node(t_xpnd **xpnd_list, t_token *tkn_curr)
{
	t_xpnd	*xpnd_curr;
	int		keep_node;

	keep_node = 0;
	if (!xpnd_list || !(*xpnd_list))
		return (true);
	xpnd_curr = *xpnd_list;
	while (xpnd_curr)
	{
		if (xpnd_curr->str_to_join[0] != '\0'
			|| xpnd_curr->in_double || xpnd_curr->in_single
			|| redir_prev_tkn_check(tkn_curr) != 0)
			keep_node++;
		xpnd_curr = xpnd_curr->next;
	}
	return (keep_node);
}

int	create_expanded_token(t_token **tkn_xpnd_list, t_token *tkn_current,
							t_xpnd **xpnd_list, char *res)
{
	t_token	*tkn_xpnd_curr;

	if (check_empty_xpnd_node(xpnd_list, tkn_current) == 0)
		return (true);
	tkn_xpnd_curr = ft_lstnewtoken_xpnd();
	if (!tkn_xpnd_curr)
		return (false);
	tkn_xpnd_curr->token_raw = gc_strdup(tkn_current->token_raw, GC_TKN);
	if (!tkn_xpnd_curr->token_raw)
		return (false);
	tkn_xpnd_curr->token_type = tkn_current->token_type;
	tkn_xpnd_curr->token_value = gc_strdup(res, GC_TKN);
	if (!tkn_xpnd_curr->token_value)
		return (false);
	ft_lstadd_back_token(tkn_xpnd_list, tkn_xpnd_curr);
	return (true);
}

int	join_xpnd(t_xpnd **xpnd_list, t_token **tkn_xpnd_list, t_token *tkn_current)
{
	t_xpnd	*xpnd_curr;
	char	*res;
	char	*temp_join;
	int		i;

	i = 0;
	if (!xpnd_list || !(*xpnd_list))
		return (true);
	xpnd_curr = *xpnd_list;
	res = gc_strdup("", GC_TKN);
	if (!res)
		return (false);
	while (xpnd_curr)
	{
		temp_join = gc_strjoin(res, xpnd_curr->str_to_join, GC_TKN);
		if (!temp_join)
			perror_free_gc("minishell: gc_strjoin malloc failure\n");
		res = temp_join;
		xpnd_curr = xpnd_curr->next;
	}
	if (!create_expanded_token(tkn_xpnd_list, tkn_current, xpnd_list, res))
		return (false);
	return (true);
}

int	process_trailing_dollar_loop(t_xpnd *xpnd_curr, t_token *tkn_current)
{
	size_t	len;

	len = ft_strlen(xpnd_curr->str_to_join);
	if (len > 0 && xpnd_curr->str_to_join[len - 1] == '$'
		&& !xpnd_curr->xpnd_check && xpnd_curr->next
		&& (xpnd_curr->next->in_single || xpnd_curr->next->in_double)
		&& !xpnd_curr->in_double && !xpnd_curr->in_single)
	{
		if (heredoc_delim_check(tkn_current) && xpnd_curr->str_to_join[len - 2]
			&& xpnd_curr->str_to_join[len - 2] == '$'
			&& trailing_dollar_count(xpnd_curr->str_to_join) % 2 == 0)
			xpnd_curr->str_to_join = gc_strndup(xpnd_curr->str_to_join, len,
					GC_TKN);
		else
			xpnd_curr->str_to_join = gc_strndup(xpnd_curr->str_to_join, len - 1,
					GC_TKN);
		if (!xpnd_curr->str_to_join)
			return (false);
	}
	return (true);
}

int	handle_dollarsign_before_quotes(t_xpnd **xpnd_list, t_token *tkn_current)
{
	t_xpnd	*xpnd_curr;

	if (!xpnd_list || !(*xpnd_list))
		return (false);
	xpnd_curr = *xpnd_list;
	while (xpnd_curr)
	{
		if (!process_trailing_dollar_loop(xpnd_curr, tkn_current))
			return (false);
		xpnd_curr = xpnd_curr->next;
	}
	return (true);
}
