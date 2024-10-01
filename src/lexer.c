
#include "../include/minishell.h"

t_token *lexer(const char *input) {
    size_t i = 0;
    t_token *tokens = NULL;

    while (input[i] != '\0') {
        skip_whitespace(input, &i);
        if (input[i] == '\0')
            break;
        if (is_operator_char(input[i])) {
            t_token_type type = identify_operator(input, &i);
            add_token(&tokens, create_token(type, NULL));
        } else {
            char *word = collect_word(input, &i);
            if (word == NULL) {
                free_tokens(tokens);
                return NULL;
            }
            add_token(&tokens, create_token(TOKEN_WORD, word));
        }
    }
    add_token(&tokens, create_token(TOKEN_EOF, NULL));
    return tokens;
}

t_token *create_token(t_token_type type, char *value) {
    t_token *token = malloc(sizeof(t_token));
    if (!token)
        return NULL;
    token->type = type;
    token->value = value;
    token->next = NULL;
    return token;
}

void add_token(t_token **tokens, t_token *new_token) {
    if (*tokens == NULL) {
        *tokens = new_token;
    } else {
        t_token *temp = *tokens;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = new_token;
    }
}

void skip_whitespace(const char *input, size_t *i) {
    while (input[*i] == ' ' || input[*i] == '\t')
        (*i)++;
}

int is_operator_char(char c) {
    return (c == '|' || c == '<' || c == '>' || c == ';');
}

char *collect_word(const char *input, size_t *i) {
    size_t start = *i;
    size_t len = 0;
    char *word = ft_strdup("");
    if (!word)
        return (NULL);

    while (input[*i] != '\0' && !isspace((unsigned char)input[*i]) && !is_operator_char(input[*i])) {
        if (input[*i] == '\'' || input[*i] == '"') {
            char *quoted = collect_quoted(input, i, input[*i]);
            if (!quoted) {
                free(word);
                return NULL;
            }
            // Append quoted string to word
            char *new_word = ft_strjoin(word, quoted);
            free(word);
            free(quoted);
            word = new_word;
            if (!word)
                return (NULL);
        } else {
            len++;
            (*i)++;
        }
    }

    // Copy the word from input
    if (len > 0) {
        char *substr = ft_substr(input, start, len);
        if (!substr) {
            free(word);
            return NULL; // Handle memory allocation failure
        }
        char *new_word = ft_strjoin(word, substr);
        free(word);
        free(substr);
        word = new_word;
        if (!word) {
            // Handle memory allocation failure
            return NULL;
        }
    }
    return word;
}

char *collect_quoted(const char *input, size_t *i, char quote_char) {
    (*i)++; // Skip the opening quote
    size_t start = *i;
    size_t len = 0;
    while (input[*i] != '\0' && input[*i] != quote_char) {
        len++;
        (*i)++;
    }
    if (input[*i] != quote_char) {
        fprintf(stderr, "Error: Unclosed quote\n");
        return NULL;
    }
    char *quoted = ft_substr(input, start, len);
    (*i)++; // Skip the closing quote
    return quoted;
}

t_token_type identify_operator(const char *input, size_t *i) {
    if (input[*i] == '|') {
        (*i)++;
        return TOKEN_PIPE;
    } else if (input[*i] == '<') {
        (*i)++;
        if (input[*i] == '<') {
            (*i)++;
            return TOKEN_HEREDOC;
        }
        return TOKEN_REDIRECT_IN;
    } else if (input[*i] == '>') {
        (*i)++;
        if (input[*i] == '>') {
            (*i)++;
            return TOKEN_APPEND;
        }
        return TOKEN_REDIRECT_OUT;
    }
    return TOKEN_EOF; // Should not reach here
}


