/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 14:24:49 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/27 22:15:50 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	set_command_name(t_parser_state *state, char *value)
{
	state->cmd_current->cmd = gc_strdup(value, GC_CMD);
	if (!state->cmd_current->cmd)
		perror_free_gc("gc_strdup malloc failure\n");
	state->cmd_current->is_builtin = is_builtin(state->shell,
			state->cmd_current->cmd);
	return (true);
}

static int	handle_word_token(t_token *tkn_current, t_parser_state *state)
{
	if (state->arg_idx >= 255)
		perror_free_gc("minishell: too many arguments in input call\n");
	state->cmd_current->args[state->arg_idx]
		= gc_strdup(tkn_current->token_value, GC_CMD);
	if (!state->cmd_current->args[state->arg_idx])
		return (false);
	if (state->arg_idx == 0)
	{
		if (!set_command_name(state, tkn_current->token_value))
			return (false);
	}
	state->arg_idx++;
	return (true);
}

static int	process_token_list(t_token *tkn_current, t_parser_state *state)
{
	while (tkn_current)
	{
		if (state->cmd_current == NULL)
			if (!init_new_cmd(state))
				return (false);
		if (tkn_current->token_type == TKN_PIPE)
		{
			if (!handle_pipe_token(state, tkn_current))
				return (false);
		}
		else if (is_redir_operator(tkn_current->token_type))
		{
			if (!handle_redir_token(&tkn_current, &state->redir_list))
				return (false);
		}
		else if (tkn_current->token_type == TKN_WORD)
			if (!handle_word_token(tkn_current, state))
				return (false);
		tkn_current = tkn_current->next;
	}
	return (true);
}

static int	finalize_cmd_list(t_parser_state *state)
{
	if (state->cmd_current != NULL)
	{
		state->cmd_current->redir = state->redir_list;
		if (state->arg_idx < 256)
			state->cmd_current->args[state->arg_idx] = NULL;
		else
			return (false);
	}
	return (true);
}

int	parse_tokens(t_cmd **cmd_list_head, t_token **tkn_list, t_shell *shell)
{
	t_parser_state	state;

	if (!tkn_list || !(*tkn_list))
		return (false);
	state.cmd_list_head = cmd_list_head;
	state.cmd_current = NULL;
	state.redir_list = NULL;
	state.arg_idx = 0;
	state.shell = shell;
	if (!process_token_list(*tkn_list, &state))
		return (false);
	return (finalize_cmd_list(&state));
}
