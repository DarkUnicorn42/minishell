/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtcza <mwojtcza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 18:20:56 by mwojtcza          #+#    #+#             */
/*   Updated: 2024/11/10 18:20:56 by mwojtcza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define INITIAL_CAPACITY 100

# include "libft/libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <string.h>
# include <ctype.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_history
{
	char	**commands;
	int		count;
	int		capacity;
}	t_history;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_SINGLE_QUOTED,
	TOKEN_DOUBLE_QUOTED,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

typedef struct s_redirection
{
	t_token_type			type;
	char					*file;
	struct s_redirection	*next;
}	t_redirection;

typedef struct s_command
{
	char				**args;
	t_redirection		*redirections;
	t_token_type		*arg_types;
	struct s_command	*next;
}	t_command;

typedef struct s_shell
{
	int			exit_code;
	char		**envp;
	char		*current_dir;
	pid_t		last_pid;
	int			input_fd;
	t_token		*tokens;
	t_command	*commands;
}	t_shell;

// minishell.c
void			print_welcome_message(void);
int				init_shell(t_shell *shell, char **envp, t_history *history);
void			process_input(char *input, t_shell *shell, t_history *history);
void			clear_shell_state(t_shell *shell);
void			free_current_dir(char **current_dir);
void			free_envp(char ***envp);
void			cleanup_shell(t_shell *shell, t_history *history);
void			initialize_shell(t_shell *shell);
void			shell_loop(t_shell *shell, t_history *history);

// history.c
int				add_history_entry(char *input, t_history *history);
int				resize_history(t_history *history);
int				add_input_to_history(char *input, t_history *history);
void			free_history(t_history *history);

// lexer.c
t_token			*lexer(const char *input);
void			handle_operator(const char *input, size_t *i, t_token **tokens);
t_token_type	handle_pipe(const char *input, size_t *i);
t_token_type	handle_less_than(const char *input, size_t *i);
t_token_type	handle_greater_than(const char *input, size_t *i);
t_token_type	identify_operator(const char *input, size_t *i);

int				handle_word(const char *input, size_t *i, t_token **tokens);
int				handle_quotes(const char *input, size_t *i,
					char **word, int *quote_t);
int				parse_word(const char *input, size_t *i,
					char **word, int *quote_type);
char			*collect_quoted(const char *input, size_t *i, char quote_char);
t_token			*create_token_quote(int quote_type, char *word);
int				handle_unquoted(const char *input, size_t *i, char **word);
t_token_type	identify_operator(const char *input, size_t *i);
t_token			*collect_word_token(const char *input, size_t *i);
t_token			*create_token(t_token_type type, char *value);
void			add_token(t_token **tokens, t_token *new_token);

// parser.c
t_command		*parse_tokens(t_token *tokens);
int				handle_redirection(t_token **tokens,
					t_command *current_command, t_command **commands);
t_command		*start_new_command(t_command **commands);
int				is_word_token(t_token_type type);
t_command		*create_command(void);
void			add_command(t_command **commands, t_command *new_command);
void			add_argument(t_command *command, char *arg, t_token_type type);
char			**realloc_arguments(char **args, int i, char *arg);
t_token_type	*realloc_arg_types(t_token_type *arg_types,
					int i, t_token_type type);
void			add_redirection(t_command *command,
					t_token_type type, char *file);
int				is_redirection(t_token_type type);
void			free_commands(t_command *commands);
void			free_arguments(char **args);
void			free_command(t_command *command);
void			free_redirections(t_redirection *redir);

// executor.c
int				execute_commands(t_command *commands,
					t_shell *shell, t_history *history);
int				execute_command(t_command *cmd, t_shell *shell, t_history *history);
int				is_builtin_parent(char *cmd);
int				is_builtin(char *cmd);
int				execute_builtin(t_command *command,
					t_shell *shell, t_history *history);
int				run_builtin_command(t_command *command,
					t_shell *shell, t_history *history);
void			execute_external(t_command *command, t_shell *shell);
char			*find_executable_path(char **paths, char *cmd);
void			execute_command_full_path(char *full_path, char **args, char **envp);
char			*search_in_path(char *cmd, char **envp);
int				check_file_access(char *full_path);
char			*get_full_path(t_command *command, t_shell *shell);
void			execute_parent(t_command *cmd, int pipe_fd[2], pid_t pid, t_shell *shell);
void			execute_child(t_command *cmd, t_shell *shell,
					t_history *history, int pipe_fd[2]);
char			*join_path_cmd(char *path, char *cmd);
int				check_executable(char *full_path, char *cmd);

// expander.c
char			*expand_variable(const char *str, int *i, t_shell *shell);
char			*expand_exit_code(t_shell *shell, int *i);
char			*expand_variables(const char *str, t_shell *shell);
char			*append_expanded_token(char *r, const char *str,
					int *i, t_shell *shell);
char			*get_expansion(const char *str, int *i, t_shell *shell);
char			*expand_argument(char *arg, t_shell *shell);

// pipes.c
int				create_pipe(int pipe_fd[2]);
pid_t			fork_process(void);
int				parent_process(int input_fd, int pipe_fd[2], t_command *cmd);
void			wait_for_children(t_shell *shell);
void			setup_child_io(t_command *cmd, int input_fd, int pipe_fd[2]);

// redirections.c
int				handle_redirections(t_command *command, t_shell *shell);
int				open_file_for_redirection(t_redirection *redir);
int				get_dup_fd(t_token_type type);
int				handle_heredoc(char *delimiter);

// builtins.c
int				builtin_echo(char **args);
int				builtin_pwd(void);
int				builtin_unset(char **args, t_shell *shell);
int				builtin_exit(char **args, t_shell *shell, t_history *history);
int				builtin_history(t_history *history);

// cd.c
int				builtin_cd(char **args, t_shell *shell);
char			*get_cd_path(char **args, t_shell *shell);
int				update_pwd(t_shell *shell);
int				count_args(char **args);

// env.c
int				builtin_env(t_shell *shell);
char			*get_env_value(const char *name, char **envp);
char			*construct_env_entry(const char *key, const char *value);
char			**set_env_val(const char *key, const char *value, char **envp);
char			**add_env_ent(char **envp, const char *key, const char *value);
char			**unset_env_value(const char *key, char **envp);
char			**realloc_envp(char **envp, int new_size);

// export.c
int				builtin_export(char **args, t_shell *shell);
int				process_export_arg(char *arg, t_shell *shell);
int				expand_envp(t_shell *shell, char *new_var);
int				update_envp(char **envp, char *key, char *new_value);
int				print_export_env(t_shell *shell);
int				parse_export_arg(char *arg, char **key, char **value);
int				print_export_id_error(char *identifier);
char			**duplicate_envp_with_space(char **envp);

// utils.c
void			print_tokens(t_token *tokens);
void			free_tokens(t_token *tokens);
char			**duplicate_envp(char **envp);
int				copy_envp(char **envp, char **new_envp, int count);
char			*ft_strncat_char(char *str, char c);
char			*join_and_free(char *str1, char *str2);
void			skip_whitespace(const char *input, size_t *i);
int				is_operator_char(char c);
int				set_exit_code(t_shell *shell, int code);
void			free_string_array(char **array);
int				is_valid_identifier(const char *str);
int				is_numeric(const char *str);
void			print_exit_error(char *arg, char *message);
int				print_error(char *message, int exit_code);

// signals.c
void			handle_sigquit(int sig);
void			handle_sigint(int sig);
void			ignore_sigquit(void);
void			setup_signals(void);

#endif
