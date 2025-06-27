/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expnd_first_sgmt.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:32:41 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/26 14:32:44 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	create_unquoted_segment(char *tkn_raw, size_t start, size_t i,
							t_xpnd **xpnd_list)
{
	t_xpnd	*current_xpnd;

	if (i <= start)
		return (true);
	current_xpnd = ft_lstnewxpnd();
	if (!current_xpnd)
		return (false);
	current_xpnd->substr = gc_strndup(tkn_raw + start, i - start, GC_TKN);
	current_xpnd->xpnd_check = false;
	ft_lstadd_back_xpnd(xpnd_list, current_xpnd);
	return (true);
}

static t_xpnd	*create_quoted_segment(t_bool in_single, t_bool in_double)
{
	t_xpnd	*current_xpnd;

	current_xpnd = ft_lstnewxpnd();
	if (!current_xpnd)
		return (NULL);
	current_xpnd->in_single = in_single;
	current_xpnd->in_double = in_double;
	current_xpnd->xpnd_check = false;
	return (current_xpnd);
}

static size_t	find_matching_quote(char *str, size_t start, char quote_char)
{
	size_t	i;

	i = start + 1;
	while (str[i] && str[i] != quote_char)
		i++;
	if (str[i] == quote_char)
		i++;
	return (i);
}

static int	process_quoted_section(char *tkn_raw, size_t *i, size_t *start,
								t_xpnd **xpnd_list)
{
	t_xpnd	*quoted_segment;
	char	quote_char;
	size_t	quote_end;
	t_bool	is_single;

	if (!create_unquoted_segment(tkn_raw, *start, *i, xpnd_list))
		return (false);
	quote_char = tkn_raw[*i];
	is_single = (quote_char == '\'');
	quoted_segment = create_quoted_segment(is_single, !is_single);
	if (!quoted_segment)
		return (false);
	*start = *i;
	quote_end = find_matching_quote(tkn_raw, *i, quote_char);
	quoted_segment->substr = gc_strndup(tkn_raw + *start,
			quote_end - *start, GC_TKN);
	if (!quoted_segment->substr)
		return (false);
	ft_lstadd_back_xpnd(xpnd_list, quoted_segment);
	*i = quote_end;
	*start = *i;
	return (true);
}

int	quotes_first_segmentation(char *tkn_raw, t_xpnd **xpnd_list)
{
	size_t	i;
	size_t	start;

	i = 0;
	start = 0;
	while (tkn_raw[i])
	{
		if (is_quote(tkn_raw[i]))
		{
			if (!process_quoted_section(tkn_raw, &i, &start, xpnd_list))
				return (false);
		}
		else
			i++;
	}
	if (!create_unquoted_segment(tkn_raw, start, i, xpnd_list))
		return (false);
	return (true);
}
