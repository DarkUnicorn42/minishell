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

#include "../include/minishell.h"

t_command *parse_tokens(t_token *tokens) {
    t_command *commands = NULL;
    t_command *current_command = NULL;

    while (tokens && tokens->type != TOKEN_EOF)
    {
        if (tokens->type == TOKEN_WORD || tokens->type == TOKEN_SINGLE_QUOTED || tokens->type == TOKEN_DOUBLE_QUOTED)
        {
            if (!current_command)
            {
                current_command = create_command();
                add_command(&commands, current_command);
            }
            add_argument(current_command, tokens->value, tokens->type);
            if (!current_command)
            {
                free_commands(commands);
                return NULL;
            }
        }
        else if (tokens->type == TOKEN_PIPE)
            current_command = NULL;
        else if (is_redirection(tokens->type))
        {
            t_token_type redir_type = tokens->type;
            tokens = tokens->next;
            if (!tokens || (tokens->type != TOKEN_WORD && tokens->type != TOKEN_SINGLE_QUOTED && tokens->type != TOKEN_DOUBLE_QUOTED))
            {
                printf("Error: Expected delimiter after redirection\n");
                free_commands(commands);
                return NULL;
            }
            add_redirection(current_command, redir_type, tokens->value);
        }
        tokens = tokens->next;
    }
    return commands;
}

t_command *create_command()
{
    t_command *cmd = malloc(sizeof(t_command));
    if (!cmd)
        return NULL;
    cmd->args = malloc(sizeof(char *) * 1);
    cmd->arg_types = malloc(sizeof(t_token_type) * 1);
    if (!cmd->args || !cmd->arg_types)
    {
        free_commands(cmd);
        return NULL;
    }
    cmd->args[0] = NULL;
    cmd->arg_types[0] = TOKEN_EOF;
    cmd->redirections = NULL;
    cmd->next = NULL;
    return cmd;
}

void add_command(t_command **commands, t_command *new_command)
{
    if (*commands == NULL)
        *commands = new_command;
    else
    {
        t_command *temp = *commands;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = new_command;
    }
}

void add_argument(t_command *command, char *arg, t_token_type type)
{
    int i = 0;

    while (command->args && command->args[i])
        i++;
    char **new_args = malloc(sizeof(char *) * (i + 2));
    t_token_type *new_arg_types = malloc(sizeof(t_token_type) * (i + 1));
    if (!new_args || !new_arg_types)
    {
        free(new_args);
        free(new_arg_types);
        return;
    }
    for (int j = 0; j < i; j++)
    {
        new_args[j] = command->args[j];
        new_arg_types[j] = command->arg_types[j];
    }
    new_args[i] = ft_strdup(arg);
    if (!new_args[i])
    {
        free(new_args);
        free(new_arg_types);
        return;
    }
    new_args[i + 1] = NULL;
    new_arg_types[i] = type;
    if (command->args)
        free(command->args);
    if (command->arg_types)
        free(command->arg_types);
    command->args = new_args;
    command->arg_types = new_arg_types;
}

void	add_redirection(t_command *command, t_token_type type, char *file)
{
	t_redirection	*redir;
	t_redirection	*temp;

	redir = malloc(sizeof(t_redirection));
	if (!redir)
		return;
	redir->type = type;
	redir->file = ft_strdup(file);
	redir->next = NULL;
	if (!command->redirections)
		command->redirections = redir;
	else
	{
		temp = command->redirections;
		while (temp->next)
			temp = temp->next;
		temp->next = redir;
	}
}

int is_redirection(t_token_type type)
{
    return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT ||
            type == TOKEN_HEREDOC || type == TOKEN_APPEND);
}

void free_arguments(char **args)
{
    if (args) {
        int i = 0;
        while (args[i]) {
            free(args[i]);
            i++;
        }
        free(args);
    }
}

void free_commands(t_command *commands)
{
    t_command *temp;

    while (commands) {
        temp = commands;
        commands = commands->next;
        free_arguments(temp->args);
        if (temp->arg_types)
        {
            free(temp->arg_types);
            temp->arg_types = NULL;
        }
        if (temp->redirections)
        {
            t_redirection *redir = temp->redirections;
            t_redirection *next_redir;
            while (redir) 
            {
                next_redir = redir->next;
                if (redir->file)
                    free(redir->file);
                free(redir);
                redir = next_redir;
            }
        }
        free(temp);
    }
}
