
#include "../../include/minishell.h"

int expand_envp(t_shell *shell, char *new_var) {
    int count = 0;

    while (shell->envp[count])
        count++;

    char **new_envp = malloc((count + 2) * sizeof(char *));
    if (!new_envp) {
        perror("malloc");
        return 1;
    }

    for (int i = 0; i < count; i++)
        new_envp[i] = shell->envp[i];

    new_envp[count] = ft_strdup(new_var);
    if (!new_envp[count]) {
        perror("malloc");
        free(new_envp);
        return 1;
    }
    new_envp[count + 1] = NULL;

    free(shell->envp);
    shell->envp = new_envp;

    return 0;
}

int update_envp(char **envp, char *key, char *new_value)
{
    for (int i = 0; envp[i]; i++) {
        if (ft_strncmp(envp[i], key, ft_strlen(key)) == 0 && envp[i][ft_strlen(key)] == '=') {
            free(envp[i]);
            envp[i] = ft_strdup(new_value);
            if (!envp[i]) {
                perror("malloc");
                return 1;
            }
            return 0;
        }
    }
    return -1;
}

int builtin_export(char **args, t_shell *shell) {
    int i = 1;
    char *equal_sign;
    int exit_code = 0;

    if (!args[1]) {
        for (int j = 0; shell->envp[j] != NULL; j++)
            printf("declare -x %s\n", shell->envp[j]);
        return 0;
    }

    while (args[i]) {
        equal_sign = ft_strchr(args[i], '=');

        if (equal_sign) {
            size_t key_length = equal_sign - args[i];
            char *key = ft_substr(args[i], 0, key_length);
            if (!key) {
                perror("malloc");
                return 1;
            }

            if (!is_valid_identifier(key)) {
                fprintf(stderr, "export: `%s': not a valid identifier\n", key);
                free(key);
                exit_code = 1;
            } else {
                if (update_envp(shell->envp, key, args[i]) == -1) {
                    if (expand_envp(shell, args[i])) {
                        free(key);
                        return 1;
                    }
                }
            }
            free(key);
        } else {
            if (!is_valid_identifier(args[i])) {
                fprintf(stderr, "export: `%s': not a valid identifier\n", args[i]);
                exit_code = 1;
            } else {
                if (update_envp(shell->envp, args[i], args[i]) == -1) {
                    if (expand_envp(shell, args[i])) {
                        return 1;
                    }
                }
            }
        }
        i++;
    }

    return exit_code;
}