/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtcza <mwojtcza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 18:29:39 by mwojtcza          #+#    #+#             */
/*   Updated: 2024/11/10 18:29:39 by mwojtcza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token_type	handle_pipe(const char *input, size_t *i)
{
	if (input[*i] == '|')
	{
		(*i)++;
		return (TOKEN_PIPE);
	}
	return (TOKEN_EOF);
}

t_token_type	handle_less_than(const char *input, size_t *i)
{
	if (input[*i] == '<')
	{
		(*i)++;
		if (input[*i] == '<')
		{
			(*i)++;
			return (TOKEN_HEREDOC);
		}
		return (TOKEN_REDIRECT_IN);
	}
	return (TOKEN_EOF);
}

t_token_type	handle_greater_than(const char *input, size_t *i)
{
	if (input[*i] == '>')
	{
		(*i)++;
		if (input[*i] == '>')
		{
			(*i)++;
			return (TOKEN_APPEND);
		}
		return (TOKEN_REDIRECT_OUT);
	}
	return (TOKEN_EOF);
}

t_token_type	identify_operator(const char *input, size_t *i)
{
	t_token_type	token_type;

	token_type = handle_pipe(input, i);
	if (token_type != TOKEN_EOF)
		return (token_type);
	token_type = handle_less_than(input, i);
	if (token_type != TOKEN_EOF)
		return (token_type);
	token_type = handle_greater_than(input, i);
	if (token_type != TOKEN_EOF)
		return (token_type);
	return (TOKEN_EOF);
}

char	*collect_quoted(const char *input, size_t *i, char quote_char)
{
	size_t	start;
	char	*quoted;

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
		return (NULL);
	(*i)++;
	return (quoted);
}
