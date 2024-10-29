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

int main(int argc, char **argv, char **envp) {
    t_shell shell;
    char *input;
    t_token *tokens;
    t_command *commands;

    (void)argc;
    (void)argv;

    // Initialize shell struct
    shell.envp = duplicate_envp(envp);
    if (!shell.envp) {
        printf("Failed to initialize environment variables\n");
        return 1;
    }
    shell.exit_code = 0; 

    //signal(SIGQUIT, handle_sigquit);
    //signal(SIGINT, handle_sigint);

    setup_signals();

    t_history history;
    history.commands = malloc(sizeof(char *) * INITIAL_CAPACITY);
    history.count = 0;
    history.capacity = INITIAL_CAPACITY;
    
    // printf("\e[38;5;113m***************************************************************\n");
	// printf("\e[38;5;113m*  .-.   .-..-..-. .-..-.    .----..-. .-..----..-.   .-.     *\n");
	// printf("\e[38;5;113m*  |  `.'  || ||  `| || |   { {__  | {_} || {_  | |   | |     *\n");
	// printf("\e[38;5;114m*  | |╲ /| || || |╲  || |   .-._} }| { } || {__ | `--.| `--.  *\n");
	// printf("\e[38;5;114m*  `-' ` `-'`-'`-' `-'`-'   `----' `-' `-'`----'`----'`----'  *\n");
	// printf("\e[38;5;114m***************************************************************\n");
	// printf("\e[38;5;115m*        :::      :::::::: ***        Hello!  Welcome to      *\n");
	// printf("\e[38;5;115m*      :+:      :+:    :+: ***          our minishell!        *\n");
	// printf("\e[38;5;115m*    +:+ +:+         +:+   ************************************\n");
	// printf("\e[38;5;116m*  +#+  +:+       +#+      ***     ★ ★ ★  Authors:  ★ ★ ★     *\n");
	// printf("\e[38;5;116m*+#+#+#+#+#+   +#+         ************************************\n");
	// printf("\e[38;5;116m*     #+#    #+#           ***            mwojtcza            *\n");
	// printf("\e[38;5;117m*    ###   ########.fr     ***            mszlicht            *\n");
	// printf("\e[38;5;117m***************************************************************\n\n");


    while (1) {
        input = readline("📟 \e[0;32m(s)hell >> \e[0m");
   // printf("env: %s\n", shell.envp[0]);

        if (input == NULL) {
            printf(" ");
            // printf("\n\n\e[0;32mExiting shell...\e[0m\n");
            // printf("\n\e[1;32m✖️ Shell closed.\e[0m\n\n");
            break;
        }
        if (input && *input) {
            add_history(input);
            if (history.count >= history.capacity) {
                history.capacity *= 2;
                history.commands = realloc(history.commands, sizeof(char *) * history.capacity);
            }
            history.commands[history.count++] = ft_strdup(input);
        }
        tokens = lexer(input);
        if (!tokens) {
            free(input);
            continue;
        }
        commands = parse_tokens(tokens);
        if (!commands)
        {
            free_tokens(tokens);
            free(input);
            continue;
        }
        execute_commands(commands, &shell, &history);
        free_commands(commands);
        free_tokens(tokens);
        free(input);
    }
    for (int i = 0; i < history.count; i++) {
        free(history.commands[i]);
    }
    free(history.commands);

    // Free the duplicated environment variables
    for (int i = 0; shell.envp[i] != NULL; i++) {
        free(shell.envp[i]);
    }
    free(shell.envp);

    return (0);
}