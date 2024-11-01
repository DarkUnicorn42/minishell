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

#include "../include/minishell.h"

void	print_tokens(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current != NULL)
	{
		printf("Token: Type = %d, Value = %s\n", current->type, current->value);
		current = current->next;
	}
}

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

char	**duplicate_envp(char **envp)
{
	int		count;
	char	**new_envp;

	count = 0;
	while (envp[count])
		count++;
	new_envp = malloc((count + 1) * sizeof(char *));
	if (!new_envp)
		return (NULL);
	if (!copy_envp(envp, new_envp, count))
		return (NULL);
	new_envp[count] = NULL;
	return (new_envp);
}

int	copy_envp(char **envp, char **new_envp, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
		{
			while (--i >= 0)
				free(new_envp[i]);
			free(new_envp);
			return (0);
		}
		i++;
	}
	return (1);
}

char *ft_strncat_char(char *str, char c) {
    size_t len = ft_strlen(str);
    char *new_str = malloc(len + 2);
    if (!new_str) {
        perror("malloc");
        return NULL;
    }
    ft_strcpy(new_str, str);
    new_str[len] = c;
    new_str[len + 1] = '\0';
    return new_str;
}

char *join_and_free(char *str1, char *str2)
{
    char *result;

    result = ft_strjoin(str1, str2);
    if (!result)
    {
        free(str1);
        free(str2);
        return NULL;
    }
    free(str1);
    free(str2);
    return result;
}

void skip_whitespace(const char *input, size_t *i)
{
    while (input[*i] == ' ' || input[*i] == '\t')
        (*i)++;
}

int is_operator_char(char c)
{
    return (c == '|' || c == '<' || c == '>' || c == ';');
}

int	set_exit_code(t_shell *shell, int code)
{
	shell->exit_code = code;
	return (code);
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

int is_valid_identifier(const char *str)
{
    int i;

    if (!str || !str[0] || (!ft_isalpha(str[0]) && str[0] != '_'))
        return (0);
    i = 1;
    while (str[i] && str[i] != '=')
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	print_exit_error(char *arg, char *message)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(message, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

int	print_error(char *message, int exit_code)
{
	ft_putstr_fd(message, STDERR_FILENO);
	return (exit_code);
}