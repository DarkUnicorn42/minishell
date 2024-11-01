
#include "../../include/minishell.h"

int	is_builtin_parent(char *cmd)
{
	if (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "export") ||
		!ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

int	is_builtin(char *cmd)
{
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "pwd") ||
		!ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "history"))
		return (1);
	return (0);
}

int execute_builtin(t_command *command, t_shell *shell, t_history *history)
{
    int i = 0;

    while (command->args[i])
    {
        if (command->arg_types[i] != TOKEN_SINGLE_QUOTED)
        {
            char *new_arg = expand_argument(command->args[i], shell);
            if (!new_arg)
            {
                free_commands(command);
                return (1);
            }
            command->args[i] = new_arg;
        }
        i++;
    }
    return run_builtin_command(command, shell, history);
}

int	run_builtin_command(t_command *command, t_shell *shell, t_history *history)
{
	if (ft_strcmp(command->args[0], "echo") == 0)
		return (builtin_echo(command->args));
	else if (ft_strcmp(command->args[0], "cd") == 0)
		return (builtin_cd(command->args, shell));
	else if (ft_strcmp(command->args[0], "pwd") == 0)
		return (builtin_pwd());
	else if (ft_strcmp(command->args[0], "export") == 0)
		return (builtin_export(command->args, shell));
	else if (ft_strcmp(command->args[0], "unset") == 0)
		return (builtin_unset(command->args, shell));
	else if (ft_strcmp(command->args[0], "env") == 0)
		return (builtin_env(shell));
	else if (ft_strcmp(command->args[0], "exit") == 0)
		return (builtin_exit(command->args, shell, history));
	else if (ft_strcmp(command->args[0], "history") == 0)
		return (builtin_history(history));
	return (0);
}
