/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expnd_lst_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:32:35 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/27 16:09:15 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* malloc failure check happens inside ft_lstnewxpnd and gc_strndup*/
t_xpnd	*create_filled_xpnd(char *src, size_t n, t_bool check, t_xpnd *q_curr)
{
	t_xpnd	*new_xpnd;

	new_xpnd = ft_lstnewxpnd();
	new_xpnd->substr = gc_strndup(src, n, GC_TKN);
	new_xpnd->xpnd_check = check;
	new_xpnd->in_single = q_curr->in_single;
	new_xpnd->in_double = q_curr->in_double;
	return (new_xpnd);
}

t_bool	is_variable_in_dquotes(char *s, size_t i, t_token *tkn)
{
	if (s[i] == '$' && s[i + 1]
		&& (s[i + 1] == '_'
			|| is_valid_keychar(s[i + 1]) || s[i + 1] == '?'
			|| s[i + 1] == '$')
		&& !heredoc_delim_check(tkn))
		return (true);
	return (false);
}

int	create_text_before_var(char *sub, t_dq_state *st,
								t_xpnd **list, t_xpnd *q_curr)
{
	t_xpnd	*new_xpnd;

	if (st->i > st->start)
	{
		new_xpnd = create_filled_xpnd(sub + st->start, st->i - st->start,
				false, q_curr);
		ft_lstadd_back_xpnd(list, new_xpnd);
	}
	st->start = ++st->i;
	return (true);
}

int	create_variable_segment(char *sub, t_dq_state *st,
								t_xpnd **list, t_xpnd *q_curr)
{
	t_xpnd	*new_xpnd;

	if (sub[st->i] == '?' || sub[st->i] == '$' || ft_isdigit(sub[st->i]))
		st->i++;
	else
	{
		while (st->i < st->strlen - 1 && is_valid_keychar(sub[st->i]))
			st->i++;
	}
	new_xpnd = create_filled_xpnd(sub + st->start, st->i - st->start, true,
			q_curr);
	ft_lstadd_back_xpnd(list, new_xpnd);
	st->start = st->i;
	return (true);
}
