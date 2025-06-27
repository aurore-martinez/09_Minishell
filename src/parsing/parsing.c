/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 15:17:55 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/27 17:30:43 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	token_operator(char *prompt, int *i, t_token **tkn_list)
{
	int	res;

	res = token_append(prompt, i, tkn_list);
	if (res != false)
		return (res);
	res = token_heredoc(prompt, i, tkn_list);
	if (res != false)
		return (res);
	res = token_pipe(prompt, i, tkn_list);
	if (res != false)
		return (res);
	res = token_out(prompt, i, tkn_list);
	if (res != false)
		return (res);
	res = token_in(prompt, i, tkn_list);
	if (res != false)
		return (res);
	return (false);
}

static int	token_word(char *prompt, int *i, t_token **tkn_list)
{
	int		start;
	t_token	*token;

	start = (*i);
	while (prompt[(*i)])
	{
		if ((is_blank(prompt[(*i)]) || is_operator_char(prompt[(*i)]))
			&& !is_inside_quotes(prompt, *i))
			break ;
		(*i)++;
	}
	token = ft_lstnewtoken(prompt + start, (*i) - start, TKN_WORD);
	if (!token)
		return (false);
	ft_lstadd_back_token(tkn_list, token);
	return (true);
}

int	tokenize_prompt(char *prompt, t_token **tkn_list)
{
	int	i;
	int	res;

	i = 0;
	while (prompt[i])
	{
		while (prompt[i] && is_blank(prompt[i]))
			i++;
		if (!prompt[i])
			break ;
		res = token_operator(prompt, &i, tkn_list);
		if (res == -1)
			return (false);
		if (res == true)
			continue ;
		if (!token_word(prompt, &i, tkn_list))
			return (false);
	}
	return (true);
}

int	parsing(char *prompt, t_cmd **cmd_list, t_shell *shell)
{
	t_token	*tkn_list;
	t_token	*tkn_xpnd_list;

	tkn_list = NULL;
	tkn_xpnd_list = NULL;
	if (!first_syntax_check(prompt, shell))
		return (false);
	if (!tokenize_prompt(prompt, &tkn_list))
		return (shell->exit_status = 0, false);
	if (!check_token(&tkn_list, shell))
		return (shell->exit_status = 2, false);
	if (!handle_expansion(&tkn_list, &tkn_xpnd_list, shell))
		return (shell->exit_status = 0, false);
	if (!parse_tokens(cmd_list, &tkn_xpnd_list, shell))
		return (shell->exit_status = 0, false);
	return (1);
}
