/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:53:36 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/27 22:14:47 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_redir	*ft_lstnewredir(void)
{
	t_redir	*new_redir;

	new_redir = gc_mem(GC_ALLOC, sizeof(t_redir), NULL, GC_CMD);
	if (!new_redir)
		perror_free_gc("ft_lstnewredir malloc failure\n");
	new_redir->file = NULL;
	new_redir->delim = NULL;
	new_redir->type = -666;
	new_redir->next = NULL;
	new_redir->prev = NULL;
	return (new_redir);
}

t_redir	*ft_lstlast_redir(t_redir *redir)
{
	while (redir && redir->next)
		redir = redir->next;
	return (redir);
}

void	ft_lstadd_back_redir(t_redir **redir, t_redir *new)
{
	t_redir	*temp;

	if (redir == NULL || new == NULL)
		return ;
	if (*redir == NULL)
	{
		*redir = new;
		new->prev = NULL;
	}
	else
	{
		temp = ft_lstlast_redir(*redir);
		new->prev = temp;
		temp->next = new;
	}
}

int	fill_redir(t_redir **redir_list, t_token *token)
{
	t_redir	*current_redir;

	current_redir = ft_lstnewredir();
	if (!current_redir)
		perror_free_gc("ft_lstnewredir malloc failure\n");
	if (token->prev && token->prev->token_type != TKN_HEREDOC)
	{
		current_redir->file = gc_strdup(token->token_value, GC_CMD);
		if (!current_redir->file)
			return (false);
	}
	else if (token->prev && token->prev->token_type == TKN_HEREDOC)
	{
		current_redir->delim = gc_strdup(token->token_value, GC_CMD);
		if (!current_redir->delim)
			return (false);
	}
	current_redir->type = token->prev->token_type;
	ft_lstadd_back_redir(redir_list, current_redir);
	return (true);
}

void	print_redir(t_cmd *temp)
{
	t_redir	*redir_current;
	int		i;

	redir_current = NULL;
	i = 0;
	if (!temp)
		return ;
	redir_current = temp->redir;
	while (redir_current)
	{
		printf("	redir_file[%d]: `%s`	;	redir_delim[%d]: `%s`	",
			i, redir_current->file, i, redir_current->delim);
		printf(";	redir_type[%d]: `%d`\n", i, redir_current->type);
		i++;
		redir_current = redir_current->next;
	}
}
