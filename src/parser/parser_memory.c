/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_memory.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtcza <mwojtcza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 18:28:36 by mwojtcza          #+#    #+#             */
/*   Updated: 2024/11/10 18:28:36 by mwojtcza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**realloc_arguments(char **args, int i, char *arg)
{
	char	**new_args;
	int		j;

	new_args = malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return (NULL);
	j = 0;
	while (j < i)
	{
		new_args[j] = args[j];
		j++;
	}
	new_args[i] = ft_strdup(arg);
	if (!new_args[i])
		return (NULL);
	new_args[i + 1] = NULL;
	free(args);
	return (new_args);
}

t_token_type	*realloc_arg_types(t_token_type *arg_types, int i,
									t_token_type type)
{
	t_token_type	*new_arg_types;
	int				j;

	new_arg_types = malloc(sizeof(t_token_type) * (i + 1));
	if (!new_arg_types)
		return (NULL);
	j = 0;
	while (j < i)
	{
		new_arg_types[j] = arg_types[j];
		j++;
	}
	new_arg_types[i] = type;
	free(arg_types);
	return (new_arg_types);
}

void	free_arguments(char **args)
{
	int	i;

	if (args)
	{
		i = 0;
		while (args[i])
		{
			free(args[i]);
			i++;
		}
		free(args);
	}
}

void	free_commands(t_command *commands)
{
	t_command	*temp;

	while (commands)
	{
		temp = commands;
		commands = commands->next;
		free_command(temp);
	}
}

void	free_command(t_command *command)
{
	free_arguments(command->args);
	if (command->arg_types)
		free(command->arg_types);
	free_redirections(command->redirections);
	free(command);
}
