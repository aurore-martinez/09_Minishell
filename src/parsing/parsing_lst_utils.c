/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_lst_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 17:36:45 by tjacquel          #+#    #+#             */
/*   Updated: 2025/06/27 22:11:37 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_cmd	*ft_lstnewcmd(void)
{
	t_cmd	*new_cmd;

	new_cmd = gc_mem(GC_ALLOC, sizeof(t_cmd), NULL, GC_CMD);
	if (!new_cmd)
		perror_free_gc("ft_lstnewcmd malloc failure\n");
	new_cmd->args = gc_mem(GC_ALLOC, (sizeof(char *) * (MAX_ARGS + 1)),
			NULL, GC_CMD);
	if (!new_cmd->args)
		perror_free_gc("ft_lstnewcmd malloc failure\n");
	new_cmd->cmd = NULL;
	new_cmd->args[0] = NULL;
	new_cmd->fd_in = -1;
	new_cmd->fd_out = -1;
	new_cmd->pipe[0] = -1;
	new_cmd->pipe[1] = -1;
	new_cmd->pid = 0;
	new_cmd->is_builtin = false;
	new_cmd->redir = NULL;
	new_cmd->prev = NULL;
	new_cmd->next = NULL;
	return (new_cmd);
}

t_cmd	*ft_lstlast_cmd(t_cmd *cmd)
{
	while (cmd && cmd->next)
		cmd = cmd->next;
	return (cmd);
}

void	ft_lstadd_back_cmd(t_cmd **cmd, t_cmd *new)
{
	t_cmd	*temp;

	if (cmd == NULL || new == NULL)
		return ;
	if (*cmd == NULL)
	{
		*cmd = new;
		new->prev = NULL;
	}
	else
	{
		temp = ft_lstlast_cmd(*cmd);
		new->prev = temp;
		temp->next = new;
	}
}

/* void	print_args(t_cmd *cmd)
{
	int		i ;
	int		j;
	t_cmd	*temp;

	i = 0;
	j = 0;
	temp = cmd;
	while (temp && temp->prev)
		temp = temp->prev;
	while (temp)
	{
		printf("-----------------\nCommand %d: `%s` ", i, temp->cmd);
		printf("; is_builtin=%d\n", temp->is_builtin);
		if (temp->args)
		{
			j = 0;
			while (temp->args[j])
			{
				printf("arg[%d]: `%s`\n", j, temp->args[j]);
				j++;
			}
		}
		if (temp->redir)
			print_redir(temp);
		i++;
		temp = temp->next;
	}
	printf("----------------- END OF COMMAND LIST -----------------\n");
} */
