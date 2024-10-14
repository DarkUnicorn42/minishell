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

void handle_sigquit(int sig)
{
    if (sig == SIGQUIT)
    {}
}

void handle_sigint(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\n📟 \e[0;32m(s)hell >> \e[0m", 28);
    rl_on_new_line();
    rl_redisplay();
}

void ignore_sigquit() {
    struct sigaction sa_quit;

    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_handler = SIG_IGN;
    sa_quit.sa_flags = 0;

    if (sigaction(SIGQUIT, &sa_quit, NULL) == -1) {
        perror("sigaction SIGQUIT");
        _exit(1);
    }
}

void setup_signals(void) {
    struct sigaction sa_int;

    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_handler = handle_sigint;
    sa_int.sa_flags = SA_RESTART;

    if (sigaction(SIGINT, &sa_int, NULL) == -1) {
        perror("sigaction SIGINT");
        _exit(1);
    }

    ignore_sigquit();
}