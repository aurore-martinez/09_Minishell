/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   functions_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:24:30 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/27 22:06:41 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	update_quote_len_state(char c, t_quote_state *st)
{
	if (c == '\'' && !st->in_double)
		st->in_single = !st->in_single;
	else if (c == '\"' && !st->in_single)
		st->in_double = !st->in_double;
	else
		st->len++;
}

size_t	ft_strnlen_noquotes(char *src, size_t n)
{
	t_quote_state	st;

	st.i = 0;
	st.len = 0;
	st.in_single = 0;
	st.in_double = 0;
	while (st.i < n && src[st.i])
	{
		if (is_quote(src[st.i]))
			update_quote_len_state(src[st.i], &st);
		else
			st.len++;
		st.i++;
	}
	return (st.len);
}

static void	update_copy_state(char c, t_quote_state *st, char *dest)
{
	if (c == '\'' && !st->in_double)
		st->in_single = !st->in_single;
	else if (c == '\"' && !st->in_single)
		st->in_double = !st->in_double;
	else
		dest[st->len++] = c;
}

char	*gc_strndup_noquotes(char *src, size_t n, t_gc_type type)
{
	char			*dest;
	t_quote_state	st;
	size_t			len_noquotes;

	len_noquotes = ft_strnlen_noquotes(src, n);
	dest = gc_mem(GC_ALLOC, len_noquotes + 1, NULL, type);
	if (!dest)
		perror_free_gc("gc_strndup_noquotes malloc failure\n");
	st.i = 0;
	st.len = 0;
	st.in_single = 0;
	st.in_double = 0;
	while (st.i < n && src[st.i])
	{
		if (is_quote(src[st.i]))
			update_copy_state(src[st.i], &st, dest);
		else
			dest[st.len++] = src[st.i];
		st.i++;
	}
	dest[st.len] = '\0';
	return (dest);
}
