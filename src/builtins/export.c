
#include "../../include/minishell.h"


int	builtin_export(char **args, t_shell *shell)
{
	int		i;
	int		exit_code;

	if (!args[1])
		return (print_export_env(shell));
	exit_code = 0;
	i = 1;
	while (args[i])
	{
		if (!process_export_arg(args[i], shell))
			exit_code = 1;
		i++;
	}
	return (exit_code);
}

int	process_export_arg(char *arg, t_shell *shell)
{
	char	*key;
	char	*value;
	int		result;

	result = parse_export_arg(arg, &key, &value);
	if (result == -1)
		return (print_error("malloc error\n", 1));
	if (result == 0)
		return (print_export_id_error(arg));
	if (update_envp(shell->envp, key, value) == -1)
	{
		if (expand_envp(shell, arg))
			return (print_error("malloc error\n", 1));
	}
	free(key);
	free(value);
	return (1);
}

int expand_envp(t_shell *shell, char *new_var)
{
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
                return (1);
            }
            return (0);
        }
    }
    return (-1);
}

int	print_export_env(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->envp[i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putendl_fd(shell->envp[i], STDOUT_FILENO);
		i++;
	}
	return (0);
}

int	parse_export_arg(char *arg, char **key, char **value)
{
	char	*equal_sign;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		*key = ft_substr(arg, 0, equal_sign - arg);
		*value = ft_strdup(equal_sign + 1);
	}
	else
	{
		*key = ft_strdup(arg);
		*value = NULL;
	}
	if (!*key || (equal_sign && !*value))
		return (-1);
	if (!is_valid_identifier(*key))
		return (0);
	return (1);
}

int	print_export_id_error(char *identifier)
{
	ft_putstr_fd("export: `", STDERR_FILENO);
	ft_putstr_fd(identifier, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	return (0);
}
