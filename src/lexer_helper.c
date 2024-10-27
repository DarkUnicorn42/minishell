

#include "../include/minishell.h"

t_token_type	identify_operator(const char *input, size_t *i)
{
	if (input[*i] == '|')
	{
		(*i)++;
		return (TOKEN_PIPE);
	}
	else if (input[*i] == '<')
	{
		(*i)++;
		if (input[*i] == '<')
		{
			(*i)++;
			return (TOKEN_HEREDOC);
		}
		return (TOKEN_REDIRECT_IN);
	}
	else if (input[*i] == '>')
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

t_token	*create_token(t_token_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **tokens, t_token *new_token)
{
	t_token	*temp;

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

void	handle_operator(const char *input, size_t *i, t_token **tokens)
{
	t_token_type	type;

	type = identify_operator(input, i);
	add_token(tokens, create_token(type, NULL));
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