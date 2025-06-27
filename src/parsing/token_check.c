/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 18:05:29 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/27 21:49:40 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	print_syntax_err(const char *s)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd((char *)s, 2);
	ft_putstr_fd("'\n", 2);
}

static const char	*token_type_string(t_token_type token_type)
{
	if (token_type == TKN_PIPE)
		return ("|");
	else if (token_type == TKN_IN)
		return ("<");
	else if (token_type == TKN_OUT)
		return (">");
	else if (token_type == TKN_APPEND)
		return (">>");
	else if (token_type == TKN_HEREDOC)
		return ("<<");
	else
		return ("WTF IS GOING ON");
}

static int	process_check_tkn(t_token *temp)
{
	if ((is_redir_operator(temp->token_type) && temp->next
			&& is_redir_operator(temp->next->token_type))
		|| (is_operator_token(temp) && temp->next
			&& temp->next->token_type == TKN_PIPE))
	{
		print_syntax_err(token_type_string(temp->next->token_type));
		return (false);
	}
	else if (is_operator_token(temp) && temp->next
		&& is_operator_token(temp->next) && temp->next->next
		&& is_operator_token(temp->next->next))
	{
		print_syntax_err(token_type_string(temp->next->token_type));
		return (false);
	}
	else if (is_operator_token(temp) && !temp->next)
	{
		ft_putstr_fd(NWL_SYNT_ERR, 2);
		return (false);
	}
	return (true);
}

int	check_token(t_token **token_list, t_shell *shell)
{
	t_token	*temp;

	if (!token_list || !(*token_list))
		return (false);
	temp = *token_list;
	while (temp->prev)
		temp = temp->prev;
	while (temp)
	{
		if (!process_check_tkn(temp))
		{
			shell->exit_status = 2;
			return (false);
		}
		temp = temp->next;
	}
	return (true);
}
