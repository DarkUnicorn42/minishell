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

    while (tokens && tokens->type != TOKEN_EOF) {
        if (tokens->type == TOKEN_WORD) {
            if (!current_command) {
                current_command = create_command();
                add_command(&commands, current_command);
            }
            add_argument(current_command, tokens->value);
        } else if (tokens->type == TOKEN_PIPE) {
            current_command = NULL; // Start a new command after a pipe
        } else if (is_redirection(tokens->type)) {
            if (!current_command) {
                fprintf(stderr, "Error: Redirection without command\n");
                free_commands(commands);
                return NULL;
            }
            tokens = tokens->next;
            if (!tokens || tokens->type != TOKEN_WORD) {
                fprintf(stderr, "Error: Expected filename after redirection\n");
                free_commands(commands);
                return NULL;
            }
            add_redirection(current_command, tokens->type, tokens->value);
        }
        tokens = tokens->next;
    }
    return commands;
}

t_command *create_command() {
    t_command *cmd = malloc(sizeof(t_command));
    if (!cmd)
        return NULL;
    cmd->args = NULL;
    cmd->redirections = NULL;
    cmd->next = NULL;
    return cmd;
}

void add_command(t_command **commands, t_command *new_command) {
    if (*commands == NULL) {
        *commands = new_command;
    } else {
        t_command *temp = *commands;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = new_command;
    }
}

void add_argument(t_command *command, char *arg) {
    int i = 0;
    while (command->args && command->args[i])
        i++;
    char **new_args = malloc(sizeof(char *) * (i + 2));
    if (!new_args)
        return;
    for (int j = 0; j < i; j++)
        new_args[j] = command->args[j];
    new_args[i] = ft_strdup(arg);
    new_args[i + 1] = NULL;
    if (command->args)
        free(command->args);
    command->args = new_args;
}

void add_redirection(t_command *command, t_token_type type, char *file) {
    t_redirection *redir = malloc(sizeof(t_redirection));
    if (!redir)
        return;
    redir->type = type;
    redir->file = ft_strdup(file);
    redir->next = NULL;

    if (!command->redirections) {
        command->redirections = redir;
    } else {
        t_redirection *temp = command->redirections;
        while (temp->next)
            temp = temp->next;
        temp->next = redir;
    }
}

int is_redirection(t_token_type type) {
    return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT ||
            type == TOKEN_HEREDOC || type == TOKEN_APPEND);
}

void free_commands(t_command *commands) {
    t_command *temp_cmd;
    t_redirection *temp_redir;
    t_redirection *redir;

    while (commands) {
        // Free each argument in the args array
        if (commands->args) {
            for (int i = 0; commands->args[i] != NULL; i++) {
                free(commands->args[i]);
            }
            free(commands->args);
        }

        // Free redirections linked list
        redir = commands->redirections;
        while (redir) {
            temp_redir = redir;
            redir = redir->next;
            if (temp_redir->file) {
                free(temp_redir->file);
            }
            free(temp_redir);
        }

        // Move to the next command
        temp_cmd = commands;
        commands = commands->next;
        free(temp_cmd);
    }
}