/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 17:41:09 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/26 17:47:29 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_operator_char(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (true);
	else
		return (false);
}

int	closed_quotes(char *prompt)
{
	int	i;
	int	in_single;
	int	in_double;

	i = 0;
	in_single = false;
	in_double = false;
	while (prompt[i])
	{
		if (prompt[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (prompt[i] == '\"' && !in_single)
			in_double = !in_double;
		i++;
	}
	return (in_single == 0 && in_double == 0);
}

int	is_inside_quotes(char *prompt, int pos)
{
	int	i;
	int	in_single;
	int	in_double;

	i = 0;
	in_single = false;
	in_double = false;
	while (i < pos && prompt[i])
	{
		if (prompt[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (prompt[i] == '\"' && !in_single)
			in_double = !in_double;
		i++;
	}
	return (in_single || in_double);
}

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

int	is_blank(int c)
{
	if (c == ' ' || c == '\t')
		return (1);
	return (0);
}
