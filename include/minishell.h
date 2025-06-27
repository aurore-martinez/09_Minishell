/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjacquel <tjacquel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:47:04 by aumartin          #+#    #+#             */
/*   Updated: 2025/06/27 22:37:38 by tjacquel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define MAX_ARGS 255

# define RED "\001\e[1;91m\002"
# define YELLOW "\001\e[1;93m\002"
# define CYAN "\001\e[1;96m\002"
# define GREEN "\001\e[1;92m\002"
# define MAGENTA "\001\e[1;95m\002"
# define RESET "\001\033[0m\002 "
# define STDIN_EOF_WARNING "minishell: warning: "
# define STDIN_EOF_MSG "here-document delimited by end-of-file (wanted `"
# define WARNING_ENV "minishell: warning: running with empty environment\n"
# define PIPE_SYNT_ERR "minishell: syntax error near unexpected token `|'\n"
# define TKN_SYNT_ERR "minishell: syntax error near unexpected token `%c'\n"
# define NWL_SYNT_ERR "minishell: syntax error near unexpected token `newline'\n"

/* ==========================    📚 INCLUDES    ========================== */

# include "../lib/ft_printf/ft_printf.h"
# include "../lib/gnl/get_next_line_bonus.h"
# include "../lib/libft/libft.h"
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

extern int				g_sig;

/* ===========================    🔷 ENUMS    =========================== */

typedef enum e_gc_action
{
	GC_ALLOC,
	GC_FREE_ONE,
	GC_FREE_ALL
}						t_gc_action;

typedef enum e_gc_type
{
	GC_NONE,
	GC_ENV,
	GC_TKN,
	GC_CMD,
	GC_TMP
}						t_gc_type;

typedef enum e_bool
{
	false,
	true
}						t_bool;

typedef enum e_token_type
{
	TKN_WORD,
	TKN_PIPE,
	TKN_IN,
	TKN_OUT,
	TKN_APPEND,
	TKN_HEREDOC,
}						t_token_type;

typedef enum e_parsing_type
{
	OPERATOR,
	LITERAL_STRING,
	CMD,
}						t_parsing_type;

/* ==========================    📦 STRUCTURES    ========================== */

typedef struct s_gc_node
{
	void				*ptr;
	t_gc_type			type;
	struct s_gc_node	*next;
}						t_gc_node;

typedef struct s_gc
{
	t_gc_node			*head;
}						t_gc;

typedef struct s_env
{
	char				*key;
	t_bool				equal;
	char				*value;
	struct s_env		*next;
}						t_env;

typedef struct s_redir
{
	t_token_type		type;
	char				*file;
	char				*delim;
	struct s_redir		*next;
	struct s_redir		*prev;
}						t_redir;

typedef struct s_cmd
{
	char				*cmd;
	char				**args;
	int					fd_in;
	int					fd_out;
	int					pid;
	int					pipe[2];
	t_bool				is_builtin;
	t_redir				*redir;
	struct s_cmd		*next;
	struct s_cmd		*prev;
}						t_cmd;

typedef struct s_shell	t_shell;
typedef struct s_cmd	t_cmd;

/* typedef de pointeur de fonction. */
typedef int				(*t_builtin_func)(t_shell *, t_cmd *, int);

typedef struct s_builtin
{
	char				*name;
	t_builtin_func		func;
}						t_builtin;

typedef struct s_std_backup
{
	int					orig_stdin;
	int					orig_stdout;
	int					orig_stderr;
}						t_std_backup;

typedef struct s_shell
{
	t_gc				gc;
	t_env				*env;
	char				**paths;
	t_builtin			builtins[8];
	int					exit_status;
	t_std_backup		std_backup;
}						t_shell;

typedef struct s_token
{
	int					token_type;
	char				*token_value;
	char				*token_raw;
	struct s_token		*prev;
	struct s_token		*next;
}						t_token;

typedef struct s_xpnd
{
	char				*substr;
	t_bool				in_single;
	t_bool				in_double;
	t_bool				xpnd_check;
	char				*str_to_join;
	struct s_xpnd		*prev;
	struct s_xpnd		*next;
}						t_xpnd;

typedef struct s_nq_state
{
	size_t				i;
	size_t				start;
}						t_nq_state;

typedef struct s_dq_state
{
	size_t				i;
	size_t				start;
	size_t				strlen;
}						t_dq_state;

typedef struct s_quote_state
{
	size_t				i;
	size_t				len;
	t_bool				in_single;
	t_bool				in_double;
}						t_quote_state;

typedef struct s_parser_state
{
	t_cmd				**cmd_list_head;
	t_cmd				*cmd_current;
	t_redir				*redir_list;
	int					arg_idx;
	t_shell				*shell;
}						t_parser_state;

/* ===========================    ♻️ PROMPT    =========================== */
void					ft_prompt(t_shell *shell);

/* ======================     🧹 GARBAGE COLLECTOR    ====================== */
void					*gc_mem(t_gc_action op, size_t size, void *ptr,
							t_gc_type type);
char					*gc_strdup(const char *src, t_gc_type type);
char					*gc_strndup(char *src, size_t n, t_gc_type type);
char					**gc_split(char *str, char sep, t_gc_type type);
char					*gc_strjoin(char const *s1, char const *s2,
							t_gc_type type);
char					*gc_itoa(int n, t_gc_type type);

/* ==============================    🛠️ UTILS    ============================ */
void					error_exit(const char *message);
char					*gc_strndup_noquotes(char *src, size_t n,
							t_gc_type type);
size_t					ft_strnlen_noquotes(char *src, size_t n);
char					*ft_strndup(char *src, size_t n);
t_token					*ft_lstnew_token(char *value, int type, int node_num);
void					print_token(t_token *token_list);
t_shell					*get_shell(void);
void					init_shell(void);
void					free_and_cleanup_heredocs(t_cmd *cmd_list);
void					perror_free_gc(const char *message);
void					print_redir(t_cmd *temp);
void					free_gc_exit(void);

/* ===========================    🚀 EXECUTION    =========================== */
void					exec_dispatcher(t_cmd *cmd, t_shell *shell);
void					exec_single_cmd(t_cmd *cmd, t_shell *shell);
char					*find_command_path(char *cmd, t_env *env);
void					print_cmd_path_found(char *cmd, t_env *env);
int						apply_redirections(t_cmd *cmd);
void					exec_pipeline(t_cmd *cmd, t_shell *shell);
void					wait_for_children(t_cmd *cmds, t_shell *shell);
t_bool					is_valid_command(t_cmd *cmd, t_shell *shell,
							int *status, char **path);
int						check_redirections_consistency(t_cmd *cmd);
int						handle_all_heredocs(t_cmd *cmd_list);
void					restore_std(t_std_backup *backup);
t_bool					is_directory(char *file);

/* ========================    🌱 ENVIRONNEMENT    ======================== */
void					print_envp(char **envp);
void					print_env_line(char *line);
void					print_env(t_env *env);
void					print_env_tab(char **env_tab);
void					env_from_envp(t_shell *shell, char **envp);
char					**env_to_env_tab_for_execve(t_env *env);
char					*get_env_value(t_env *env, const char *key,
							t_gc_type type);
void					update_env_value(t_env *env, const char *key,
							const char *new_value);
t_env					*env_new(char *key, char *value, t_bool equal);
void					env_add_back(t_env **lst, t_env *new);

/* ===========================    🔧 BUILTINS    ============================ */
int						ft_pwd(t_shell *shell, t_cmd *cmd, int fd);
int						ft_env(t_shell *shell, t_cmd *cmd, int fd);
int						ft_echo(t_shell *shell, t_cmd *cmd, int fd);
int						ft_export(t_shell *shell, t_cmd *cmd, int fd);
void					print_env_export(t_env *env);
int						is_valid_identifier(char *arg);
t_env					*check_existing_key(t_shell *shell, char *key);
int						ft_unset(t_shell *shell, t_cmd *cmd, int fd);
int						ft_cd(t_shell *shell, t_cmd *cmd, int fd);
int						ft_exit(t_shell *shell, t_cmd *cmd, int fd);
int						handle_builtin(t_shell *shell, t_cmd *cmd, int fd);
t_bool					is_builtin(t_shell *shell, char *cmd_name);

/* ========================    🦄 PARSING    ======================== */
int						parsing(char *prompt, t_cmd **cmd_list, t_shell *shell);
void					ft_lstadd_back_token(t_token **token, t_token *new);
t_token					*ft_lstnewtoken(char *prompt, int n,
							t_token_type token_type);
int						is_operator_token(t_token *token);
int						check_token(t_token **token_list, t_shell *shell);
int						is_quote(char c);
int						parse_tokens(t_cmd **cmd_list, t_token **tkn_list,
							t_shell *shell);
t_cmd					*ft_lstnewcmd(void);
void					ft_lstadd_back_cmd(t_cmd **cmd, t_cmd *new);
int						is_inside_squotes(char *token_raw);
int						is_inside_dquotes(char *token_raw);
int						is_redir_operator(t_token_type token_type);
int						fill_redir(t_redir **redir, t_token *token);
int						is_inside_dquotes(char *token_raw);
int						is_inside_squotes(char *token_raw);
int						is_blank(int c);
int						token_in(char *prompt, int *i, t_token **tkn_list);
int						token_out(char *prompt, int *i, t_token **tkn_list);
int						token_pipe(char *prompt, int *i, t_token **tkn_list);
int						token_heredoc(char *prompt, int *i, t_token **tkn_list);
int						token_append(char *prompt, int *i, t_token **tkn_list);
int						first_syntax_check(char *prompt, t_shell *shell);
int						is_inside_quotes(char *prompt, int pos);
int						closed_quotes(char *prompt);
int						is_operator_char(char c);
int						init_new_cmd(t_parser_state *state);
int						handle_pipe_token(t_parser_state *state,
							t_token *tkn_current);
int						handle_redir_token(t_token **tkn_ptr,
							t_redir **redir_list_ptr);

/* ========================    💰 EXPANSION    ======================== */
int						handle_expansion(t_token **tkn_list,
							t_token **tkn_xpnd_list, t_shell *shell);
t_xpnd					*ft_lstnewxpnd(void);
void					ft_lstadd_back_xpnd(t_xpnd **xpnd, t_xpnd *new);
t_xpnd					*ft_lstlast_xpnd(t_xpnd *xpnd);
t_token					*ft_lstnewtoken_xpnd(void);
void					free_t_xpnd_list(t_xpnd *xpnd_quotes_list);
int						is_valid_keychar(char c);
int						is_valid_keychar(char c);
int						heredoc_delim_check(t_token *tkn_curr);
int						redir_prev_tkn_check(t_token *tkn_curr);
void					printf_xpnd(t_xpnd **xpnd_list);
int						trailing_dollar_count(char *str);
int						handle_dollarsign_before_quotes(t_xpnd **xpnd_list,
							t_token *tkn_current);
int						process_trailing_dollar_loop(t_xpnd *xpnd_curr,
							t_token *tkn_current);
int						join_xpnd(t_xpnd **xpnd_list, t_token **tkn_xpnd_list,
							t_token *tkn_current);
int						create_expanded_token(t_token **tkn_xpnd_list,
							t_token *tkn_current, t_xpnd **xpnd_list,
							char *res);
int						check_empty_xpnd_node(t_xpnd **xpnd_list,
							t_token *tkn_curr);
int						quotes_first_segmentation(char *tkn_raw,
							t_xpnd **xpnd_list);
t_bool					is_variable_in_dquotes(char *s, size_t i, t_token *tkn);
int						dquotes_scnd_segmentation(char *substr,
							t_xpnd *xpnd_quotes_curr, t_xpnd **xpnd_list,
							t_token *tkn_curr);
t_xpnd					*create_filled_xpnd(char *src, size_t n, t_bool check,
							t_xpnd *q_curr);
int						scnd_segmentation(t_xpnd *xpnd_quotes_curr,
							t_xpnd **xpnd_list, t_token *tkn_current);
int						create_variable_segment(char *sub, t_dq_state *st,
							t_xpnd **list, t_xpnd *q_curr);
int						create_text_before_var(char *sub, t_dq_state *st,
							t_xpnd **list, t_xpnd *q_curr);

/* ========================    📡 SIGNALS    ======================== */
void					init_signals(void);
void					signal_handler(int sig);
void					signal_handler_exec(int sig);
void					set_signals_interactive(void);
void					set_signals_exec(void);
void					signal_handler_heredoc(int sig);

/* ========================    🚧 DEBUG    ======================== */
void					debug_env(t_shell *shell);
void					debug_env_tab(t_shell *shell);
void					debug_path_for(char *cmd, t_shell *shell);
void					debug_cmd(t_cmd *cmd);
void					debug_cmd_list(t_cmd *cmds);
void					debug_pipe(int in_fd, int pipe_fd[2]);
void					debug_exec_external(t_cmd *cmd);

#endif
