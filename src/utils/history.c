/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtcza <mwojtcza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 18:29:08 by mwojtcza          #+#    #+#             */
/*   Updated: 2024/11/10 18:29:08 by mwojtcza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	add_history_entry(char *input, t_history *history)
{
	add_history(input);
	if (history->count >= history->capacity)
	{
		if (!resize_history(history))
			return (0);
	}
	history->commands[history->count] = ft_strdup(input);
	if (!history->commands[history->count])
		return (0);
	history->count++;
	return (1);
}

int	resize_history(t_history *history)
{
	char	**new_commands;
	int		j;
	int		new_capacity;

	new_capacity = history->capacity * 2;
	new_commands = malloc(sizeof(char *) * new_capacity);
	if (!new_commands)
		return (0);
	j = 0;
	while (j < history->count)
	{
		new_commands[j] = history->commands[j];
		j++;
	}
	while (j < new_capacity)
	{
		new_commands[j] = NULL;
		j++;
	}
	free(history->commands);
	history->commands = new_commands;
	history->capacity = new_capacity;
	return (1);
}

int	add_input_to_history(char *input, t_history *history)
{
	if (input && *input)
	{
		if (!add_history_entry(input, history))
		{
			ft_putstr_fd("Memory allocation failed for command history\n",
				STDERR_FILENO);
			free(input);
			return (0);
		}
	}
	return (1);
}

void	free_history(t_history *history)
{
	int	i;

	if (history->commands)
	{
		i = 0;
		while (i < history->count)
		{
			free(history->commands[i]);
			i++;
		}
		free(history->commands);
		history->commands = NULL;
	}
}
