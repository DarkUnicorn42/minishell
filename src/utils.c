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
