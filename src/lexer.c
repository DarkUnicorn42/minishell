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

t_token	*lexer(const char *input)
{
	t_token	*tokens;
	size_t	i;

	tokens = NULL;
	i = 0;
	while (input[i] != '\0')
	{
		skip_whitespace(input, &i);
		if (input[i] == '\0')
			break ;
		if (is_operator_char(input[i]))
			handle_operator(input, &i, &tokens);
		else
		{
			if (!handle_word(input, &i, &tokens))
			{
				free_tokens(tokens);
				return (NULL);
			}
		}
	}
	add_token(&tokens, create_token(TOKEN_EOF, NULL));
	return (tokens);
}

t_token *collect_word_token(const char *input, size_t *i)
{
    char        *word;
    t_token     *token;
    int         quote_type; // 0: unquoted, 1: single-quoted, 2: double-quoted

    word = ft_strdup("");
    if (!word)
        return (NULL);
    quote_type = 0;
    while (input[*i] != '\0' && !isspace((unsigned char)input[*i]) &&
           !is_operator_char(input[*i]))
    {
        if (input[*i] == '\'' || input[*i] == '"')
        {
            if (!handle_quotes(input, i, &word, &quote_type))
            {
                free(word);
                return (NULL);
            }
        }
        else
        {
            if (!handle_unquoted(input, i, &word))
            {
                free(word);
                return (NULL);
            }
        }
    }
    if (quote_type == 1)
        token = create_token(TOKEN_SINGLE_QUOTED, word);
    else if (quote_type == 2)
        token = create_token(TOKEN_DOUBLE_QUOTED, word);
    else
        token = create_token(TOKEN_WORD, word);
    if (!token)
    {
        free(word);
        return (NULL);
    }
    return (token);
}

int handle_quotes(const char *input, size_t *i, char **word, int *quote_type)
{
    char    *quoted_content;
    char    *temp;
    char    quote_char;

    quote_char = input[*i];
    quoted_content = collect_quoted(input, i, quote_char);
    if (!quoted_content)
        return (0);
    temp = join_and_free(*word, quoted_content);
    if (!temp)
    {
        free(quoted_content);
        free(*word);
        return (0);
    }
    *word = temp;
    if (quote_char == '\'' && *quote_type != 2)
        *quote_type = 1;
    else if (quote_char == '"' && *quote_type != 1)
        *quote_type = 2;
    return (1);
}

char *collect_quoted(const char *input, size_t *i, char quote_char)
{
    size_t  start;
    char    *quoted;

    (*i)++;
    start = *i;
    while (input[*i] != '\0' && input[*i] != quote_char)
        (*i)++;
    if (input[*i] != quote_char)
    {
        ft_putstr_fd("Error: Unclosed quote\n", STDERR_FILENO);
        return (NULL);
    }
    quoted = ft_substr(input, start, *i - start);
    if (!quoted)
    {
        free(quoted);
        return (NULL);
    }
    else
        (*i)++;
    return (quoted);
}

int	handle_unquoted(const char *input, size_t *i, char **word)
{
	size_t	start;
	char	*substr;
	char	*temp;

	start = *i;
	while (input[*i] != '\0' && !isspace((unsigned char)input[*i]) &&
		   !is_operator_char(input[*i]) && input[*i] != '\'' && input[*i] != '"')
	{
		(*i)++;
	}
	substr = ft_substr(input, start, *i - start);
	if (!substr)
		return (0);
	temp = join_and_free(*word, substr);
	if (!temp)
	{
        free(*word);
		free(substr);
		return (0);
	}
    else
	    *word = temp;
	return (1);
}
