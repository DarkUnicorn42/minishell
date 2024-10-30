/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtcza <mwojtcza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:04:18 by mwojtcza          #+#    #+#             */
/*   Updated: 2024/10/03 15:07:27 by mwojtcza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_command	*parse_tokens(t_token *tokens)
{
	t_command	*commands;
	t_command	*current_command;

	commands = NULL;
	current_command = NULL;
	while (tokens && tokens->type != TOKEN_EOF)
	{
		if (is_word_token(tokens->type))
		{
			if (!current_command)
				current_command = start_new_command(&commands);
			if (!current_command)
				return (NULL);
			add_argument(current_command, tokens->value, tokens->type);
		}
		else if (tokens->type == TOKEN_PIPE)
			current_command = NULL;
		else if (is_redirection(tokens->type))
		{
			if (!handle_redirection(&tokens, current_command, &commands))
				return (NULL);
		}
		tokens = tokens->next;
	}
	return (commands);
}

t_command	*start_new_command(t_command **commands)
{
	t_command	*new_command;

	new_command = create_command();
	if (!new_command)
	{
		free_commands(*commands);
		return (NULL);
	}
	add_command(commands, new_command);
	return (new_command);
}

t_command	*create_command()
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = malloc(sizeof(char *));
	cmd->arg_types = malloc(sizeof(t_token_type));
	if (!cmd->args || !cmd->arg_types)
	{
		free_command(cmd);
		return (NULL);
	}
	cmd->args[0] = NULL;
	cmd->redirections = NULL;
	cmd->next = NULL;
	return (cmd);
}

void	add_command(t_command **commands, t_command *new_command)
{
	t_command	*temp;

	if (*commands == NULL)
		*commands = new_command;
	else
	{
		temp = *commands;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new_command;
	}
}

void	add_argument(t_command *command, char *arg, t_token_type type)
{
	int				i;
	char			**new_args;
	t_token_type	*new_arg_types;

	i = 0;
	while (command->args && command->args[i])
		i++;
	new_args = realloc_arguments(command->args, i, arg);
	new_arg_types = realloc_arg_types(command->arg_types, i, type);
	if (!new_args || !new_arg_types)
	{
		free(new_args);
		free(new_arg_types);
		return ;
	}
	command->args = new_args;
	command->arg_types = new_arg_types;
}
