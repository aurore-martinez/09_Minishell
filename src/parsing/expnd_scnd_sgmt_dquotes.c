/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expnd_scnd_sgmt_dquotes.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:33:03 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/27 16:08:40 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	handle_dquote_variable(char *sub, t_dq_state *st,
								t_xpnd **list, t_xpnd *q_curr)
{
	if (!create_text_before_var(sub, st, list, q_curr))
		return (false);
	if (!create_variable_segment(sub, st, list, q_curr))
		return (false);
	return (true);
}

static int	init_dquote_state(t_dq_state *st, t_xpnd *xpnd_quotes_curr)
{
	st->strlen = ft_strlen(xpnd_quotes_curr->substr);
	if (st->strlen < 2)
		return (false);
	st->i = 1;
	st->start = 1;
	return (true);
}

static int	handle_empty_dquotes(t_dq_state *st, t_xpnd **xpnd_list,
									t_xpnd *xpnd_quotes_curr)
{
	t_xpnd	*new_xpnd;

	if (st->strlen <= 2)
	{
		new_xpnd = create_filled_xpnd("", 0, false, xpnd_quotes_curr);
		ft_lstadd_back_xpnd(xpnd_list, new_xpnd);
		return (true);
	}
	return (false);
}

static void	handle_final_segment(char *substr, t_dq_state *st,
								t_xpnd **xpnd_list, t_xpnd *xpnd_quotes_curr)
{
	t_xpnd	*new_xpnd;

	if (st->i > st->start)
	{
		new_xpnd = create_filled_xpnd(substr + st->start, st->i - st->start,
				false, xpnd_quotes_curr);
		ft_lstadd_back_xpnd(xpnd_list, new_xpnd);
	}
}

int	dquotes_scnd_segmentation(char *substr, t_xpnd *xpnd_quotes_curr,
								t_xpnd **xpnd_list, t_token *tkn_curr)
{
	t_dq_state	st;

	if (!init_dquote_state(&st, xpnd_quotes_curr))
		return (true);
	if (handle_empty_dquotes(&st, xpnd_list, xpnd_quotes_curr))
		return (true);
	while (st.i < st.strlen - 1)
	{
		if (is_variable_in_dquotes(substr, st.i, tkn_curr))
		{
			if (!handle_dquote_variable(substr, &st, xpnd_list,
					xpnd_quotes_curr))
				return (false);
		}
		else
			st.i++;
	}
	handle_final_segment(substr, &st, xpnd_list, xpnd_quotes_curr);
	return (true);
}
