

#ifndef MINISHELL_H
# define MINISHELL_H

#define INITIAL_CAPACITY 100

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


typedef struct s_shell {
    char **envp;
    int exit_code;
    char *current_dir;  // Track current working directory
} t_shell;

typedef struct s_history {
    char **commands;    // Tablica komend
    int count;          // Liczba zapisanych komend
    int capacity;       // Pojemność tablicy
} t_history;

typedef enum {
    TOKEN_WORD,
    TOKEN_SINGLE_QUOTED,
    TOKEN_DOUBLE_QUOTED,
    TOKEN_PIPE,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_APPEND,
    TOKEN_HEREDOC,
    TOKEN_EOF
} t_token_type;

typedef struct s_token {
    t_token_type    type;
    char            *value;
    struct s_token  *next;
} t_token;

typedef struct s_redirection {
    t_token_type    type;     // <, >, <<, >>
    char            *file;    // File name for the redirection
    struct s_redirection *next;
} t_redirection;

typedef struct s_command {
    char            **args;        // Command and arguments (e.g., {"ls", "-la", NULL})
    t_redirection   *redirections; // Linked list of redirections
    t_token_type    *arg_types;    // Array of token types for each argument
    struct s_command *next;        // For a chain of commands connected by pipes
} t_command;


//lexer.c
t_token *lexer(const char *input);
t_token *collect_word_token(const char *input, size_t *i);
t_token_type identify_operator(const char *input, size_t *i);
t_token *collect_quoted(const char *input, size_t *i, char quote);
t_token *create_token(t_token_type type, char *value);
void add_token(t_token **tokens, t_token *new_token);

// parser.c
t_command *parse_tokens(t_token *tokens);
t_command *create_command();
void add_command(t_command **commands, t_command *new_command);
void add_argument(t_command *command, char *arg, t_token_type type);
void add_redirection(t_command *command, t_token_type type, char *file);
int is_redirection(t_token_type type);
void free_commands(t_command *commands);

// executor.c
int execute_commands(t_command *commands, t_shell *shell, t_history *history);
int	is_builtin_parent(char *cmd);
int	is_builtin(char *cmd);
int	execute_builtin(t_command *command, t_shell *shell, t_history *history);
int	run_builtin_command(t_command *command, t_shell *shell, t_history *history);
void	execute_external(t_command *command, t_shell *shell);
char	*find_executable_path(char **paths, char *cmd);


//expander.c
char *expand_variable(const char *str, int *i, t_shell *shell);
char *expand_exit_code(t_shell *shell, int *i);
char *expand_variables(const char *str, t_shell *shell);
char	*append_expanded_token(char *result, const char *str, int *i, t_shell *shell);
char	*get_expansion(const char *str, int *i, t_shell *shell);
char	*expand_argument(char *arg, t_shell *shell);

//pipes.c
int create_pipe(int pipe_fd[2]);
pid_t	fork_process(void);
void	execute_child(t_command *cmd, t_shell *shell, t_history *history, int input_fd, int pipe_fd[2]);
int	parent_process(int input_fd, int pipe_fd[2], t_command *cmd);
void	wait_for_children(t_shell *shell, pid_t last_pid);
void	setup_child_io(t_command *cmd, int input_fd, int pipe_fd[2]);

// redirections.c
int	handle_redirections(t_command *command);
int	open_file_for_redirection(t_redirection *redir);
int	get_dup_fd(t_token_type type);
int	handle_heredoc(char *delimiter);

// builtins.c
int builtin_echo(char **args);
int builtin_cd(char **args, t_shell *shell);
int builtin_pwd();
int builtin_unset(char **args, t_shell *shell);
int	builtin_exit(char **args, t_shell *shell);
int builtin_history(t_history *history);

// env.c
int builtin_env(t_shell *shell);
char *get_env_value(const char *name, char **envp);
char	*construct_env_entry(const char *key, const char *value);
char **set_env_value(const char *key, const char *value, char **envp);
char	**add_env_entry(char **envp, const char *key, const char *value);
char **unset_env_value(const char *key, char **envp);

// export.c
int expand_envp(t_shell *shell, char *new_var);
int update_envp(char **envp, char *key, char *new_value);
int builtin_export(char **args, t_shell *shell);

//utils.c
void print_tokens(t_token *tokens);
void free_tokens(t_token *tokens);
char **duplicate_envp(char **envp);
char *ft_strncat_char(char *str, char c);
char *join_and_free(char *str1, const char *str2);
void skip_whitespace(const char *input, size_t *i);
int is_operator_char(char c);
int	set_exit_code(t_shell *shell, int code);
void	free_string_array(char **array);
int is_valid_identifier(const char *str);
int	is_numeric(const char *str);

//signals.c
void handle_sigquit(int sig);
void handle_sigint(int sig);
void ignore_sigquit(void);
void setup_signals(void);

#endif
/* 
minishell.c: Contains the main loop and initialization.

lexer.c: Functions for tokenizing the input string.

parser.c: Functions for parsing tokens into command structures.

expander.c: Functions for expanding environment variables.

executor.c: Functions for executing commands and handling built-ins.

builtins.c: Implementations of built-in shell commands.

redirections.c: Functions for handling input/output redirections.

pipes.c: Functions for setting up and managing pipes between processes.

memory.c: Memory allocation and deallocation functions.

utils.c: Utility functions used across the program.
*/