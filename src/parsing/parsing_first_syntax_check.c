/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_first_syntax_check.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 17:42:29 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/27 21:21:24 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	check_leading_pipe(char *prompt)
{
	int	i;

	i = 0;
	while (prompt[i] && is_blank(prompt[i]))
		i++;
	if (prompt[i] == '|' && !is_inside_quotes(prompt, i))
	{
		ft_putstr_fd(PIPE_SYNT_ERR, 2);
		return (false);
	}
	return (true);
}

static int	check_pipe_sequence(char *prompt, int *i)
{
	if (prompt[*i + 1] && prompt[*i + 1] == '|')
	{
		ft_putstr_fd(PIPE_SYNT_ERR, 2);
		return (false);
	}
	(*i)++;
	while (prompt[*i] && is_blank(prompt[*i]))
		(*i)++;
	if (prompt[*i] == '|' || !prompt[*i])
	{
		ft_putstr_fd(PIPE_SYNT_ERR, 2);
		return (false);
	}
	return (true);
}

int	pipes_check(char *prompt)
{
	int	i;

	if (!check_leading_pipe(prompt))
		return (false);
	i = 0;
	while (prompt[i])
	{
		if (prompt[i] == '|' && !is_inside_quotes(prompt, i))
		{
			if (!check_pipe_sequence(prompt, &i))
				return (false);
		}
		else
			i++;
	}
	return (true);
}

int	first_syntax_check(char *prompt, t_shell *shell)
{
	if (!closed_quotes(prompt))
	{
		ft_putstr_fd("minishell: syntax error: open quotes\n", 2);
		shell->exit_status = 2;
		return (false);
	}
	if (!pipes_check(prompt))
	{
		shell->exit_status = 2;
		return (false);
	}
	return (true);
}
