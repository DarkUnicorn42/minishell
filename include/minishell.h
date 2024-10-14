

#ifndef MINISHELL_H
# define MINISHELL_H

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
} t_shell;

typedef enum e_token_type {
    TOKEN_WORD,
    TOKEN_PIPE,            // |
    TOKEN_REDIRECT_IN,     // <
    TOKEN_REDIRECT_OUT,    // >
    TOKEN_HEREDOC,         // <<
    TOKEN_APPEND,          // >>
    TOKEN_EOF              // End of input
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
    struct s_command *next;        // For a chain of commands connected by pipes
} t_command;

//lexer.c
t_token *lexer(const char *input);
t_token *create_token(t_token_type type, char *value);
void add_token(t_token **tokens, t_token *new_token);
void skip_whitespace(const char *input, size_t *i);
int is_operator_char(char c);
char *collect_word(const char *input, size_t *i);
char *handle_quotes(const char *input, size_t *i, char *word);
char *append_substring(const char *input, size_t start, size_t len, char *word);
char *collect_quoted(const char *input, size_t *i, char quote_char);
t_token_type identify_operator(const char *input, size_t *i);
void free_tokens(t_token *tokens);

// parser.c
t_command *parse_tokens(t_token *tokens);
t_command *create_command();
void add_command(t_command **commands, t_command *new_command);
void add_argument(t_command *command, char *arg);
void add_redirection(t_command *command, t_token_type type, char *file);
int is_redirection(t_token_type type);
void free_commands(t_command *commands);

// executor.c
int execute_commands(t_command *commands, t_shell *shell);
int handle_redirections(t_command *command);
int is_builtin(char *command);
int execute_builtin(t_command *command, t_shell *shell);
char *expand_variables(const char *str, t_shell *shell);

//executor.c
int execute_commands(t_command *commands, t_shell *shell);

// builtins.c
int builtin_echo(char **args);
int builtin_cd(char **args, t_shell *shell);
int builtin_pwd();
int builtin_unset(char **args);
int builtin_env(void);
int builtin_exit(char **args);
// export.c
int expand_envp(t_shell *shell, char *new_var);
int update_envp(char **envp, char *key, char *new_value);
int builtin_export(char **args, t_shell *shell);

//utils.c
void print_tokens(t_token *tokens);
void free_tokens(t_token *tokens);
char **duplicate_envp(char **envp);
char *ft_strncat_char(char *str, char c);

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