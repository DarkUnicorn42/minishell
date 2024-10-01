

#ifndef MINISHELL_H
# define MINISHELL_H


# include "libft/libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <string.h>
# include <ctype.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

#endif

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

//lexer.c
t_token *lexer(const char *input);
t_token *create_token(t_token_type type, char *value);
void add_token(t_token **tokens, t_token *new_token);
void skip_whitespace(const char *input, size_t *i);
int is_operator_char(char c);
char *collect_word(const char *input, size_t *i);
char *collect_quoted(const char *input, size_t *i, char quote_char);
t_token_type identify_operator(const char *input, size_t *i);
void free_tokens(t_token *tokens);


//utils.c
void print_tokens(t_token *tokens);
void free_tokens(t_token *tokens);


/* 
minishell.c: Contains the main loop and initialization.

lexer.c: Functions for tokenizing the input string.

parser.c: Functions for parsing tokens into command structures.

expander.c: Functions for expanding environment variables.

executor.c: Functions for executing commands and handling built-ins.

builtins.c: Implementations of built-in shell commands.

redirections.c: Functions for handling input/output redirections.

pipes.c: Functions for setting up and managing pipes between processes.

signals.c: Signal handlers and related functions.

memory.c: Memory allocation and deallocation functions.

utils.c: Utility functions used across the program.
*/