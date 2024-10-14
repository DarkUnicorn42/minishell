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
			free(temp->value);
		free(temp);
	}
}

char **duplicate_envp(char **envp) {
    int count = 0;
    while (envp[count])
        count++;

    char **new_envp = malloc((count + 1) * sizeof(char *));
    if (!new_envp)
        return NULL;

    for (int i = 0; i < count; i++) {
        new_envp[i] = strdup(envp[i]);
        if (!new_envp[i]) {
            // Handle memory allocation failure by freeing already allocated memory
            while (--i >= 0)
                free(new_envp[i]);
            free(new_envp);
            return NULL;
        }
    }
    new_envp[count] = NULL;

    return new_envp;
}

char *ft_strncat_char(char *str, char c) {
    size_t len = ft_strlen(str);
    char *new_str = malloc(len + 2);  // +1 for the new char, +1 for the null terminator
    if (!new_str) {
        perror("malloc");
        return NULL;
    }
    ft_strcpy(new_str, str);
    new_str[len] = c;
    new_str[len + 1] = '\0';
    return new_str;
}

char *join_and_free(char *str1, const char *str2)
{
    char *result;

    result = ft_strjoin(str1, str2);
    free(str1);
    return result;
}