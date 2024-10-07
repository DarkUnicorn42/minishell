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

int main(void) {
    char *input;
    t_token *tokens;
    t_command *commands;

    // Signal handling setup
//    setup_signals();

    while (1) {
        input = readline("📟 \e[0;32m(s)hell >> \e[0m");
        if (input == NULL) {
            printf("\n\n\e[0;32mExiting shell...\e[0m\n");
            printf("\n\e[1;32m✖️ Shell closed.\e[0m\n\n");
            break;
        }
        if (input && *input)
            add_history(input);

        // Call lexer to tokenize the input
        tokens = lexer(input);
        if (!tokens) {
            printf(stderr, "Lexer error: Failed to tokenize input\n");
            free(input);
            continue;
        }

        // Call parser to parse tokens into commands
        commands = parse_tokens(tokens);
        if (!commands) {
            printf(stderr, "Parser error: Failed to parse tokens\n");
            free_tokens(tokens);
            free(input);
            continue;
        }

        // Execute the parsed commands
        execute_commands(commands);

        // Free allocated resources
        free_commands(commands);
        free_tokens(tokens);
        free(input);
    }
    return 0;
}