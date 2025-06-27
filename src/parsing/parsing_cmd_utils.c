/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 18:31:09 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/26 18:33:53 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	init_new_cmd(t_parser_state *state)
{
	state->cmd_current = ft_lstnewcmd();
	if (!state->cmd_current)
		return (false);
	ft_lstadd_back_cmd(state->cmd_list_head, state->cmd_current);
	state->arg_idx = 0;
	state->redir_list = NULL;
	return (true);
}

int	handle_pipe_token(t_parser_state *state, t_token *tkn_current)
{
	if (state->arg_idx < 256)
		state->cmd_current->args[state->arg_idx] = NULL;
	else
		return (false);
	state->cmd_current->redir = state->redir_list;
	state->cmd_current = NULL;
	if (!tkn_current->next || tkn_current->next->token_type == TKN_PIPE)
	{
		state->shell->exit_status = 2;
		return (false);
	}
	return (true);
}

int	handle_redir_token(t_token **tkn_ptr, t_redir **redir_list_ptr)
{
	t_token	*tkn_current;

	tkn_current = *tkn_ptr;
	if (tkn_current->next && tkn_current->next->token_type == TKN_WORD)
	{
		*tkn_ptr = tkn_current->next;
		if (!fill_redir(redir_list_ptr, *tkn_ptr))
			return (false);
	}
	return (true);
}
