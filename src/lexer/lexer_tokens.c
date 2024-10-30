
#include "../../include/minishell.h"

t_token	*collect_word_token(const char *input, size_t *i)
{
	char	*word;
	t_token	*token;
	int		quote_type;

	word = ft_strdup("");
	if (!word)
		return (NULL);
	quote_type = 0;
	if (!parse_word(input, i, &word, &quote_type))
	{
		free(word);
		return (NULL);
	}
	token = create_token_quote(quote_type, word);
	if (!token)
	{
		free(word);
		return (NULL);
	}
	return (token);
}

int	parse_word(const char *input, size_t *i, char **word, int *quote_type)
{
	while (input[*i] != '\0' && !isspace((unsigned char)input[*i])
		&& !is_operator_char(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			if (!handle_quotes(input, i, word, quote_type))
				return (0);
		}
		else
		{
			if (!handle_unquoted(input, i, word))
				return (0);
		}
	}
	return (1);
}

t_token	*create_token_quote(int quote_type, char *word)
{
	if (quote_type == 1)
		return (create_token(TOKEN_SINGLE_QUOTED, word));
	else if (quote_type == 2)
		return (create_token(TOKEN_DOUBLE_QUOTED, word));
	else
		return (create_token(TOKEN_WORD, word));
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
