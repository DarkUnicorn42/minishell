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

#include "../../include/minishell.h"

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

int	handle_word(const char *input, size_t *i, t_token **tokens)
{
	t_token	*new_token;

	new_token = collect_word_token(input, i);
	if (!new_token)
		return (0);
	add_token(tokens, new_token);
	return (1);
}

void	handle_operator(const char *input, size_t *i, t_token **tokens)
{
	t_token_type	type;
	t_token			*new_token;

	type = identify_operator(input, i);
	new_token = create_token(type, NULL);
	if (!new_token)
	{
		free_tokens(*tokens);
		return ;
	}
	add_token(tokens, new_token);
}

int	handle_quotes(const char *input, size_t *i, char **word, int *quote_t)
{
	char	*quoted_content;
	char	*temp;
	char	quote_char;

	quote_char = input[*i];
	quoted_content = collect_quoted(input, i, quote_char);
	if (!quoted_content)
		return (0);
	temp = join_and_free(*word, quoted_content);
	if (!temp)
	{
		free(*word);
		return (0);
	}
	*word = temp;
	if (quote_char == '\'' && *quote_t != 2)
		*quote_t = 1;
	else if (quote_char == '"' && *quote_t != 1)
		*quote_t = 2;
	return (1);
}

int	handle_unquoted(const char *input, size_t *i, char **word)
{
	size_t	start;
	char	*substr;
	char	*temp;

	start = *i;
	while (input[*i] != '\0' && !isspace((unsigned char)input[*i])
		&& !is_operator_char(input[*i]) && input[*i] != '\''
		&& input[*i] != '"')
		(*i)++;
	substr = ft_substr(input, start, *i - start);
	if (!substr)
		return (0);
	temp = join_and_free(*word, substr);
	if (!temp)
	{
		free(*word);
		return (0);
	}
	*word = temp;
	return (1);
}
