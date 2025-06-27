/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expnd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:33:08 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/26 15:11:05 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_valid_keychar(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (1);
	else
		return (0);
}

int	heredoc_delim_check(t_token *tkn_curr)
{
	if (tkn_curr->prev && tkn_curr->prev->token_type == TKN_HEREDOC)
		return (true);
	return (false);
}

int	redir_prev_tkn_check(t_token *tkn_curr)
{
	if (tkn_curr->prev && (tkn_curr->prev->token_type == TKN_APPEND
			|| tkn_curr->prev->token_type == TKN_IN
			|| tkn_curr->prev->token_type == TKN_OUT))
		return (true);
	return (false);
}

void	printf_xpnd(t_xpnd **xpnd_list)
{
	t_xpnd	*xpnd_current;
	int		i;

	i = 0;
	xpnd_current = *xpnd_list;
	if (!xpnd_current)
		return ;
	while (xpnd_current && xpnd_current->prev)
		xpnd_current = xpnd_current->prev;
	while (xpnd_current)
	{
		printf("xpnd_curr->substr[%d]: `%s`	;	in_single: %d	;	", i,
			xpnd_current->substr, xpnd_current->in_single);
		printf("in_double: %d	;	xpnd_check: %d	;	str_to_join: `%s`\n",
			xpnd_current->in_double, xpnd_current->xpnd_check,
			xpnd_current->str_to_join);
		i++;
		xpnd_current = xpnd_current->next;
	}
}

int	trailing_dollar_count(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == '$')
			count++;
		else
			count = 0;
		i++;
	}
	return (count);
}
