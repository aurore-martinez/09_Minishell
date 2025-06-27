/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expnd_scnd_sgmt.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:42:41 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/26 14:58:37 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_bool	is_variable_start(char *s, size_t i, t_token *tkn)
{
	if (s[i] == '$' && s[i + 1] && (s[i + 1] == '_'
			|| is_valid_keychar(s[i + 1]) || s[i + 1] == '?'
			|| s[i + 1] == '$') && !heredoc_delim_check(tkn))
		return (true);
	return (false);
}

static int	handle_variable_segment(char *sub, t_nq_state *st, t_xpnd **list,
									t_xpnd *q_curr)
{
	t_xpnd	*new_xpnd;

	if (st->i > st->start)
	{
		new_xpnd = create_filled_xpnd(sub + st->start, st->i - st->start,
				false, q_curr);
		if (!new_xpnd)
			return (false);
		ft_lstadd_back_xpnd(list, new_xpnd);
	}
	st->start = ++st->i;
	if (sub[st->i] == '?' || sub[st->i] == '$' || ft_isdigit(sub[st->i]))
		st->i++;
	else
	{
		while (sub[st->i] && is_valid_keychar(sub[st->i]))
			st->i++;
	}
	new_xpnd = create_filled_xpnd(sub + st->start, st->i - st->start, true,
			q_curr);
	if (!new_xpnd)
		return (false);
	ft_lstadd_back_xpnd(list, new_xpnd);
	st->start = st->i;
	return (true);
}

static int	noquotes_scnd_segmentation(char *substr, t_xpnd *xpnd_quotes_curr,
								t_xpnd **xpnd_list, t_token *tkn_curr)
{
	t_nq_state	st;
	t_xpnd		*new_xpnd;

	st.i = 0;
	st.start = 0;
	while (substr[st.i])
	{
		if (is_variable_start(substr, st.i, tkn_curr))
		{
			if (!handle_variable_segment(substr, &st, xpnd_list,
					xpnd_quotes_curr))
				return (false);
		}
		else
			st.i++;
	}
	if (st.i > st.start)
	{
		new_xpnd = create_filled_xpnd(substr + st.start, st.i - st.start,
				false, xpnd_quotes_curr);
		if (!new_xpnd)
			return (false);
		ft_lstadd_back_xpnd(xpnd_list, new_xpnd);
	}
	return (true);
}

static int	squotes_scnd_segmentation(char *substr, t_xpnd *xpnd_quotes_curr,
								t_xpnd **xpnd_list)
{
	size_t	strlen;
	t_xpnd	*new_xpnd;

	strlen = ft_strlen(substr);
	if (strlen >= 2)
	{
		new_xpnd = create_filled_xpnd(substr + 1, strlen - 2, false,
				xpnd_quotes_curr);
		if (!new_xpnd)
			return (false);
		ft_lstadd_back_xpnd(xpnd_list, new_xpnd);
	}
	return (true);
}

int	scnd_segmentation(t_xpnd *xpnd_quotes_curr, t_xpnd **xpnd_list,
						t_token *tkn_current)
{
	if (!xpnd_quotes_curr || !xpnd_quotes_curr->substr)
		return (true);
	if (xpnd_quotes_curr->in_single)
	{
		if (!squotes_scnd_segmentation(xpnd_quotes_curr->substr,
				xpnd_quotes_curr, xpnd_list))
			return (false);
	}
	else if (xpnd_quotes_curr->in_double)
	{
		if (!dquotes_scnd_segmentation(xpnd_quotes_curr->substr,
				xpnd_quotes_curr, xpnd_list, tkn_current))
			return (false);
	}
	else
	{
		if (!noquotes_scnd_segmentation(xpnd_quotes_curr->substr,
				xpnd_quotes_curr, xpnd_list, tkn_current))
			return (false);
	}
	return (true);
}
