/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtcza <mwojtcza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:04:23 by mwojtcza          #+#    #+#             */
/*   Updated: 2024/10/03 15:04:23 by mwojtcza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_tokens(t_token *tokens)
{
	t_token	*temp;

	while (tokens)
	{
		temp = tokens;
		tokens = tokens->next;
		if (temp->value)
		{
			free(temp->value);
			temp->value = NULL;
		}
		free(temp);
		temp = NULL;
	}
}

char	*ft_strncat_char(char *str, char c)
{
	size_t	len;
	char	*new_str;

	len = ft_strlen(str);
	new_str = malloc(len + 2);
	if (!new_str)
	{
		perror("malloc");
		return (NULL);
	}
	ft_strcpy(new_str, str);
	new_str[len] = c;
	new_str[len + 1] = '\0';
	return (new_str);
}

char	*join_and_free(char *str1, char *str2)
{
	char	*result;

	result = ft_strjoin(str1, str2);
	if (!result)
	{
		free(str1);
		free(str2);
		return (NULL);
	}
	free(str1);
	free(str2);
	return (result);
}

void	skip_whitespace(const char *input, size_t *i)
{
	while (input[*i] == ' ' || input[*i] == '\t')
		(*i)++;
}

void	free_string_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	free(array);
	array = NULL;
}
