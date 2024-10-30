/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtcza <mwojtcza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:03:54 by mwojtcza          #+#    #+#             */
/*   Updated: 2024/10/03 15:03:54 by mwojtcza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_welcome_message(void)
{
	printf("\e[38;5;113m***************************************************************\n");
	printf("\e[38;5;113m*  .-.   .-..-..-. .-..-.    .----..-. .-..----..-.   .-.     *\n");
	printf("\e[38;5;113m*  |  `.'  || ||  `| || |   { {__  | {_} || {_  | |   | |     *\n");
	printf("\e[38;5;114m*  | |╲ /| || || |╲  || |   .-._} }| { } || {__ | `--.| `--.  *\n");
	printf("\e[38;5;114m*  `-' ` `-'`-'`-' `-'`-'   `----' `-' `-'`----'`----'`----'  *\n");
	printf("\e[38;5;114m***************************************************************\n");
	printf("\e[38;5;115m*        :::      :::::::: ***        Hello!  Welcome to      *\n");
	printf("\e[38;5;115m*      :+:      :+:    :+: ***          our minishell!        *\n");
	printf("\e[38;5;115m*    +:+ +:+         +:+   ************************************\n");
	printf("\e[38;5;116m*  +#+  +:+       +#+      ***     ★ ★ ★  Authors:  ★ ★ ★     *\n");
	printf("\e[38;5;116m*+#+#+#+#+#+   +#+         ************************************\n");
	printf("\e[38;5;116m*     #+#    #+#           ***            mwojtcza            *\n");
	printf("\e[38;5;117m*    ###   ########.fr     ***            mszlicht            *\n");
	printf("\e[38;5;117m***************************************************************\n\n");
}

int	init_shell(t_shell *shell, char **envp, t_history *history)
{
	shell->envp = duplicate_envp(envp);
	if (!shell->envp)
	{
		printf("Failed to initialize environment variables\n");
		return (1);
	}
	shell->exit_code = 0;
	setup_signals();
	history->commands = malloc(sizeof(char *) * INITIAL_CAPACITY);
	if (!history->commands)
	{
		printf("Failed to allocate history\n");
		return (1);
	}
	history->count = 0;
	history->capacity = INITIAL_CAPACITY;
	//print_welcome_message();
	return (0);
}

void	process_input(char *input, t_shell *shell, t_history *history)
{
	t_token		*tokens;
	t_command	*commands;

	if (input && *input)
	{
		if (!add_history_entry(input, history))
		{
			ft_putstr_fd("Memory allocation failed for command history\n",
						 STDERR_FILENO);
			free(input);
			return ;
		}
	}
	tokens = lexer(input);
	if (!tokens)
	{
		free(input);
		return ;
	}
	commands = parse_tokens(tokens);
	if (!commands)
	{
		free_tokens(tokens);
		free(input);
		return ;
	}
	execute_commands(commands, shell, history);
	free_commands(commands);
	free_tokens(tokens);
	free(input);
}

void	cleanup_shell(t_shell *shell, t_history *history)
{
	int	i;

	i = 0;
	while (i < history->count)
	{
		free(history->commands[i]);
		i++;
	}
	free(history->commands);
	i = 0;
	while (shell->envp[i] != NULL)
	{
		free(shell->envp[i]);
		i++;
	}
	free(shell->envp);
}

// void	cleanup_shell(t_shell *shell, t_history *history)
// {
// 	int	i;

// 	// Free command history
// 	if (history->commands)
// 	{
// 		i = 0;
// 		while (i < history->count)
// 		{
// 			free(history->commands[i]);
// 			i++;
// 		}
// 		free(history->commands);
// 		history->commands = NULL;
// 	}

// 	// Free shell environment variables
// 	if (shell->envp)
// 	{
// 		i = 0;
// 		while (shell->envp[i])
// 		{
// 			free(shell->envp[i]);
// 			i++;
// 		}
// 		free(shell->envp);
// 		shell->envp = NULL;
// 	}

// 	// Free current_dir if allocated
// 	if (shell->current_dir)
// 	{
// 		free(shell->current_dir);
// 		shell->current_dir = NULL;
// 	}

// 	// Add any other necessary cleanup here
// } 

int	main(int argc, char **argv, char **envp)
{
	t_shell		shell;
	t_history	history;
	char		*input;

	(void)argc;
	(void)argv;
	if (init_shell(&shell, envp, &history))
		return (1);
	while (1)
	{
		input = readline("📟 \e[0;32m(s)hell >> \e[0m");
		if (input == NULL)
		{
			printf(" ");
			// printf("\n\n\e[0;32mExiting shell...\e[0m\n");
			// printf("\n\e[1;32m✖️ Shell closed.\e[0m\n\n");
			break ;
		}
		process_input(input, &shell, &history);
	}
	cleanup_shell(&shell, &history);
	return (0);
}
