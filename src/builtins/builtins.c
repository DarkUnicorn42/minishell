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
    char cwd[PATH_MAX];
    int	arg_count;

	arg_count = 0;
	while (args[arg_count])
		arg_count++;
	if (arg_count > 2)
	{
		ft_putstr_fd("cd: too many arguments\n", STDERR_FILENO);
		return (1);
	}
    if (!args[1]) {
        path = get_env_value("HOME", shell->envp);
        if (!path) {
            fprintf(stderr, "cd: HOME not set\n");
            return 1;
        }
    } else {
        path = args[1];
    }

    // Attempt to change the directory
    if (chdir(path) == -1) {
        perror("cd failed");
        return 1;
    }

    // Update PWD in shell->envp
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        shell->envp = set_env_value("PWD", cwd, shell->envp);
        if (!shell->envp) {
            fprintf(stderr, "Error updating PWD\n");
            return 1;
        }
    } else {
        perror("getcwd failed");
        return 1;
    }

    return 0;
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

int builtin_unset(char **args, t_shell *shell) {
    int i = 1;
    int exit_code = 0;

    while (args[i]) {
        if (!is_valid_identifier(args[i])) {
            fprintf(stderr, "unset: '%s': not a valid identifier\n", args[i]);
            exit_code = 1;
        } else {
            shell->envp = unset_env_value(args[i], shell->envp);
        }
        i++;
    }

    return exit_code;
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