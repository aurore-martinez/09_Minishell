/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_operator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 15:59:50 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/26 16:27:19 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	token_append(char *prompt, int *i, t_token **tkn_list)
{
	t_token	*token;

	if (prompt[*i] == '>' && prompt[(*i) + 1] == '>')
	{
		token = ft_lstnewtoken(prompt + *i, 2, TKN_APPEND);
		if (!token)
			return (-1);
		ft_lstadd_back_token(tkn_list, token);
		(*i) += 2;
		return (true);
	}
	return (false);
}

int	token_heredoc(char *prompt, int *i, t_token **tkn_list)
{
	t_token	*token;

	if (prompt[*i] == '<' && prompt[(*i) + 1] == '<')
	{
		token = ft_lstnewtoken(prompt + *i, 2, TKN_HEREDOC);
		if (!token)
			return (-1);
		ft_lstadd_back_token(tkn_list, token);
		(*i) += 2;
		return (true);
	}
	return (false);
}

int	token_pipe(char *prompt, int *i, t_token **tkn_list)
{
	t_token	*token;

	if (prompt[*i] == '|')
	{
		token = ft_lstnewtoken(prompt + *i, 1, TKN_PIPE);
		if (!token)
			return (-1);
		ft_lstadd_back_token(tkn_list, token);
		(*i)++;
		return (true);
	}
	return (false);
}

int	token_out(char *prompt, int *i, t_token **tkn_list)
{
	t_token	*token;

	if (prompt[*i] == '>')
	{
		token = ft_lstnewtoken(prompt + *i, 1, TKN_OUT);
		if (!token)
			return (-1);
		ft_lstadd_back_token(tkn_list, token);
		(*i)++;
		return (true);
	}
	return (false);
}

int	token_in(char *prompt, int *i, t_token **tkn_list)
{
	t_token	*token;

	if (prompt[*i] == '<')
	{
		token = ft_lstnewtoken(prompt + *i, 1, TKN_IN);
		if (!token)
			return (-1);
		ft_lstadd_back_token(tkn_list, token);
		(*i)++;
		return (true);
	}
	return (false);
}
