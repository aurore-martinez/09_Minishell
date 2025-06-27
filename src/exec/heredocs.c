/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 12:53:38 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/27 22:06:41 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*gen_tmp_filename(void)
{
	static int	id = 0;
	char		*id_str;
	char		*tmp_path;
	char		*filename;

	tmp_path = NULL;
	id_str = gc_itoa(id++, GC_TMP);
	if (!id_str)
		return (NULL);
	tmp_path = gc_strjoin("/tmp/minishell_heredoc_", id_str, GC_TMP);
	if (!tmp_path)
		return (NULL);
	filename = gc_strjoin(tmp_path, ".tmp", GC_TMP);
	if (!filename)
		return (NULL);
	return (filename);
}

static void	handle_eof_signal(t_redir *redir, size_t len, int fd)
{
	if (g_sig == 0)
	{
		write(2, STDIN_EOF_WARNING, ft_strlen(STDIN_EOF_WARNING));
		write(2, STDIN_EOF_MSG, ft_strlen(STDIN_EOF_MSG));
		write(2, redir->delim, len);
		write(2, "')\n", 3);
		return ;
	}
	if (g_sig)
	{
		g_sig = 0;
		close(fd);
		gc_mem(GC_FREE_ALL, 0, NULL, GC_NONE);
		exit(SIGINT);
	}
}

int	heredoc_childhood(t_redir *redir)
{
	char	*line;
	int		fd;

	fd = open(redir->file, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
		perror_free_gc("minishell : error : fd = -1");
	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
		{
			handle_eof_signal(redir, ft_strlen(redir->delim), fd);
			break ;
		}
		if (ft_strncmp(line, redir->delim, ft_strlen(redir->delim)) == 0
			&& line[ft_strlen(redir->delim)] == '\0')
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	close(fd);
	return (0);
}

/*
--> gere un heredoc pour une redirection de type <<
--> cree un fichier temp, lit ligne par ligne jusqu'au delimateur ou interruption
et ecrit le contenu dans ce fichier
*/
int	handle_heredoc(t_redir *redir)
{
	pid_t	pid;
	int		status;

	redir->file = gen_tmp_filename();
	if (!redir->file)
		perror_free_gc("");
	pid = fork();
	if (pid < 0)
		perror_free_gc("");
	if (pid == 0)
	{
		signal(SIGINT, signal_handler_heredoc);
		signal(SIGQUIT, SIG_IGN);
		heredoc_childhood(redir);
		gc_mem(GC_FREE_ALL, 0, NULL, GC_NONE);
		exit (0);
	}
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	init_signals();
	if (status)
		return (-1);
	return (0);
}

int	handle_all_heredocs(t_cmd *cmd_list)
{
	t_cmd	*cmd;
	t_redir	*redir;

	cmd = cmd_list;
	while (cmd)
	{
		redir = cmd->redir;
		while (redir)
		{
			if (redir->type == TKN_HEREDOC)
			{
				if (handle_heredoc(redir) == -1)
					return (-1);
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
