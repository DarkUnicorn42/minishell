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
	printf("\e[38;5;113m*************************************\n");
	printf("\e[38;5;113m*  .----..-. .-..----..-.   .-.     *\n");
	printf("\e[38;5;113m* { {__  | {_} || {_  | |   | |     *\n");
	printf("\e[38;5;114m* .-._} }| { } || {__ | `--.| `--.  *\n");
	printf("\e[38;5;114m* `----' `-' `-'`----'`----'`----'  *\n");
	printf("\e[38;5;114m*************************************\n");
	printf("\e[38;5;115m****        Hello!  Welcome to      *\n");
	printf("\e[38;5;115m****          our minishell!        *\n");
	printf("\e[38;5;115m*************************************\n");
	printf("\e[38;5;116m****     ★ ★ ★  Authors:  ★ ★ ★     *\n");
	printf("\e[38;5;116m*************************************\n");
	printf("\e[38;5;116m****            mwojtcza            *\n");
	printf("\e[38;5;117m****            mszlicht            *\n");
	printf("\e[38;5;117m*************************************\n\n");
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

void process_input(char *input, t_shell *shell, t_history *history)
{
    if (shell->tokens)
    {
        free_tokens(shell->tokens);
        shell->tokens = NULL;
    }
    if (shell->commands)
    {
        free_commands(shell->commands);
        shell->commands = NULL;
    }
    if (input && *input)
    {
        if (!add_history_entry(input, history))
        {
            ft_putstr_fd("Memory allocation failed for command history\n", STDERR_FILENO);
            free(input);
            return;
        }
    }
    shell->tokens = lexer(input);
    if (!shell->tokens)
    {
        free(input);
        return;
    }
    shell->commands = parse_tokens(shell->tokens);
    if (!shell->commands)
    {
        free_tokens(shell->tokens);
        shell->tokens = NULL;
        free(input);
        return;
    }
    execute_commands(shell->commands, shell, history);
	if (shell->tokens)
    {
        free_tokens(shell->tokens);
        shell->tokens = NULL;
    }
    if (shell->commands)
    {
        free_commands(shell->commands);
        shell->commands = NULL;
    }
    free(input);
}

void cleanup_shell(t_shell *shell, t_history *history)
{
    int i;

    if (history->commands)
    {
        for (i = 0; i < history->count; i++)
        {
            free(history->commands[i]);
        }
        free(history->commands);
        history->commands = NULL;
    }
    if (shell->envp)
    {
        i = 0;
        while (shell->envp[i])
        {
            free(shell->envp[i]);
            i++;
        }
        free(shell->envp);
        shell->envp = NULL;
    }
    if (shell->current_dir)
    {
        free(shell->current_dir);
        shell->current_dir = NULL;
    }
    if (shell->tokens)
    {
        free_tokens(shell->tokens);
        shell->tokens = NULL;
    }
    if (shell->commands)
    {
        free_commands(shell->commands);
        shell->commands = NULL;
    }
}

int main(int argc, char **argv, char **envp)
{
    t_shell     shell;
    t_history   history;
    char        *input;

    (void)argc;
    (void)argv;
    shell.tokens = NULL;
    shell.commands = NULL;
	shell.current_dir = NULL;

    if (init_shell(&shell, envp, &history))
        return (1);
    while (1)
    {
        input = readline("📟 \e[0;32m(s)hell >> \e[0m");
        if (input == NULL)
        {
            printf(" ");
            // User pressed Ctrl+D
            cleanup_shell(&shell, &history);
            break;
        }
        process_input(input, &shell, &history);
    }
    return (0);
}
