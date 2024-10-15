/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtcza <mwojtcza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:03:59 by mwojtcza          #+#    #+#             */
/*   Updated: 2024/10/03 15:04:09 by mwojtcza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int builtin_echo(char **args) {
    int i = 1;
    int newline = 1;

    // Check if -n flag is present
    if (args[i] && ft_strcmp(args[i], "-n") == 0) {
        newline = 0;
        i++;
    }

    // Print arguments
    while (args[i]) {
        printf("%s", args[i]);
        if (args[i + 1])
            printf(" ");
        i++;
    }

    if (newline)
        printf("\n");

    return (0);
}

int builtin_cd(char **args, t_shell *shell) {
    char *path;
    (void)shell;
    // Handle 'cd' with no arguments (go to HOME)
    if (!args[1])
    {
        path = getenv("HOME");
        if (!path) {
            fprintf(stderr, "cd: HOME not set\n");
            return 1;
        }
    }
    else
        path = args[1];

    if (chdir(path) == -1)
    {
        perror("cd failed");
        return 1;
    }
    return (0);
}

int builtin_pwd(void) {
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
        return 0;
    } else {
        perror("pwd");
        return (1);
    }
}

int builtin_unset(char **args) {
    if (!args[1]) {
        fprintf(stderr, "unset: missing argument\n");
        return 1;
    }
    if (unsetenv(args[1]) != 0) {
        perror("unset");
        return (1);
    }
    return (0);
}

int builtin_env(void) {
    extern char **environ;
    for (int i = 0; environ[i]; i++) {
        printf("%s\n", environ[i]);
    }
    return 0;
}

int builtin_exit(char **args) {
    int status = 0;
    if (args[1]) {
        status = ft_atoi(args[1]);
    }
    exit(status);
}

int builtin_history(t_history *history) {
    for (int i = 0; i < history->count; i++) {
        printf("%d %s\n", i + 1, history->commands[i]);
    }
    return 0;
}