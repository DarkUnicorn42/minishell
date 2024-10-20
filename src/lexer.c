/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtcza <mwojtcza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:04:14 by mwojtcza          #+#    #+#             */
/*   Updated: 2024/10/03 15:04:14 by mwojtcza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token *lexer(const char *input)
{
    t_token *tokens = NULL;
    size_t i = 0;

    while (input[i] != '\0')
    {
        skip_whitespace(input, &i);
        if (input[i] == '\0')
            break;

        if (is_operator_char(input[i]))
        {
            t_token_type type = identify_operator(input, &i);
            add_token(&tokens, create_token(type, NULL));
        }
        else
        {
            t_token *new_token = collect_word_token(input, &i);
            if (!new_token)
            {
                free_tokens(tokens);
                return NULL;
            }
            add_token(&tokens, new_token);
        }
    }

    add_token(&tokens, create_token(TOKEN_EOF, NULL));
    return tokens;
}
// kill me please
t_token *collect_word_token(const char *input, size_t *i)
{
    char *word = ft_strdup("");
    t_token *tokens = NULL;

    if (!word)
        return NULL;

    while (input[*i] != '\0' && !isspace((unsigned char)input[*i]) && !is_operator_char(input[*i]))
    {
        if (input[*i] == '\'' || input[*i] == '"')
        {
            char quote_char = input[*i];

            if (input[*i + 1] == quote_char)
            {
                if ((*i == 0 || isspace((unsigned char)input[*i - 1])) &&
                    (input[*i + 2] == '\0' || isspace((unsigned char)input[*i + 2])))
                {
                    (*i) += 2;
                    add_token(&tokens, create_token(TOKEN_WORD, ft_strdup("")));
                }
                else
                {
                    (*i) += 2;
                }
                continue;
            }

            t_token *quoted_token = collect_quoted(input, i, input[*i]);
            if (!quoted_token)
            {
                free(word);
                return NULL;
            }

            if (*word != '\0')
            {
                add_token(&tokens, create_token(TOKEN_WORD, word));
                word = ft_strdup("");
                if (!word)
                {
                    free_tokens(tokens);
                    return NULL;
                }
            }

            if (tokens && tokens->type == TOKEN_WORD)
            {
                char *temp = join_and_free(tokens->value, quoted_token->value);
                if (!temp)
                {
                    free_tokens(tokens);
                    return NULL;
                }
                tokens->value = temp;
            }
            else
            {
                add_token(&tokens, quoted_token);
            }
        }
        else
        {
            size_t start = *i;
            while (input[*i] != '\0' && !isspace((unsigned char)input[*i]) &&
                   !is_operator_char(input[*i]) && input[*i] != '\'' && input[*i] != '"')
            {
                (*i)++;
            }

            char *substr = ft_substr(input, start, *i - start);
            if (!substr)
            {
                free(word);
                return NULL;
            }

            char *temp = join_and_free(word, substr);
            if (!temp)
            {
                free(word);
                return NULL;
            }
            word = temp;
        }
    }

    if (*word != '\0')
    {
        add_token(&tokens, create_token(TOKEN_WORD, word));
    }
    else
    {
        free(word);
    }

    return tokens;
}

t_token *collect_quoted(const char *input, size_t *i, char quote_char)
{
    t_token_type type;
    (*i)++;
    size_t start = *i;

    while (input[*i] != '\0' && input[*i] != quote_char)
        (*i)++;

    if (input[*i] != quote_char)
    {
        fprintf(stderr, "Error: Unclosed quote\n");
        return NULL;
    }

    char *quoted = ft_substr(input, start, *i - start);
    if (!quoted)
        return NULL;

    (*i)++;

    type = (quote_char == '"') ? TOKEN_WORD : TOKEN_SINGLE_QUOTED;

    return create_token(type, quoted);
}

t_token_type identify_operator(const char *input, size_t *i)
{
    if (input[*i] == '|')
    {
        (*i)++;
        return TOKEN_PIPE;
    }
    else if (input[*i] == '<')
    {
        (*i)++;
        if (input[*i] == '<')
        {
            (*i)++;
            return TOKEN_HEREDOC;
        }
        return TOKEN_REDIRECT_IN;
    }
    else if (input[*i] == '>')
    {
        (*i)++;
        if (input[*i] == '>')
        {
            (*i)++;
            return TOKEN_APPEND;
        }
        return TOKEN_REDIRECT_OUT;
    }
    return TOKEN_EOF;
}

t_token *create_token(t_token_type type, char *value)
{
    t_token *token = malloc(sizeof(t_token));
    if (!token)
        return NULL;
    token->type = type;
    token->value = value;
    token->next = NULL;
    return token;
}

void add_token(t_token **tokens, t_token *new_token)
{
    t_token *temp;

    if (*tokens == NULL)
        *tokens = new_token;
    else
    {
        temp = *tokens;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = new_token;
    }
}
