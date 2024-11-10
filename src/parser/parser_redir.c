/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtcza <mwojtcza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 18:28:57 by mwojtcza          #+#    #+#             */
/*   Updated: 2024/11/10 18:28:57 by mwojtcza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_redirection(t_token **tokens, t_command *current_command,
						t_command **commands)
{
	t_token_type	redir_type;

	redir_type = (*tokens)->type;
	(*tokens) = (*tokens)->next;
	if (!(*tokens) || !is_word_token((*tokens)->type))
	{
		ft_putstr_fd("Error: Expected delimiter after redirection\n",
			STDERR_FILENO);
		free_commands(*commands);
		return (0);
	}
	if (!current_command)
	{
		current_command = start_new_command(commands);
		if (!current_command)
			return (0);
	}
	add_redirection(current_command, redir_type, (*tokens)->value);
	return (1);
}

void	add_redirection(t_command *command, t_token_type type, char *file)
{
	t_redirection	*redir;
	t_redirection	*temp;

	redir = malloc(sizeof(t_redirection));
	if (!redir)
		return ;
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

int	is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_HEREDOC || type == TOKEN_APPEND);
}

int	is_word_token(t_token_type type)
{
	return (type == TOKEN_WORD || type == TOKEN_SINGLE_QUOTED
		|| type == TOKEN_DOUBLE_QUOTED);
}
