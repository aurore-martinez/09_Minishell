/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 17:43:55 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/26 18:28:15 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_inside_dquotes(char *token_raw)
{
	int	i;
	int	in_single;
	int	in_double;

	i = 0;
	in_double = false;
	in_single = false;
	while (token_raw[i])
	{
		if (token_raw[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (token_raw[i] == '\"' && !in_single)
			in_double = !in_double;
		i++;
	}
	return (in_double);
}

int	is_inside_squotes(char *token_raw)
{
	int	i;
	int	in_single;
	int	in_double;

	i = 0;
	in_double = false;
	in_single = false;
	while (token_raw[i])
	{
		if (token_raw[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (token_raw[i] == '\"' && !in_single)
			in_double = !in_double;
		i++;
	}
	return (in_single);
}

int	is_operator_token(t_token *token)
{
	if (!token)
		return (false);
	if (token->token_type == TKN_PIPE
		|| token->token_type == TKN_IN
		|| token->token_type == TKN_OUT
		||token->token_type == TKN_APPEND
		|| token->token_type == TKN_HEREDOC)
		return (true);
	else
		return (false);
}

int	is_redir_operator(t_token_type token_type)
{
	if (token_type == TKN_IN || token_type == TKN_OUT
		|| token_type == TKN_APPEND
		|| token_type == TKN_HEREDOC)
		return (true);
	return (false);
}
