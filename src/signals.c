/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mszlicht <mszlicht@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 18:08:13 by mszlicht          #+#    #+#             */
/*   Updated: 2024/10/10 18:47:32 by mszlicht         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//(Ctrl+\)
void handle_sigquit(int sig)
{
    if (sig == SIGQUIT)
    {
        printf("\n\n\e[0;32mExiting shell...\e[0m\n");
        printf("\n\e[1;32m✖️ Shell closed.\e[0m\n\n");
        exit(0);
    }
}

//(Ctrl+C)
void handle_sigint(int sig)
{
    (void)sig;
    write(1, "\n📟 \e[0;32m(s)hell >> \e[0m", 28);
}