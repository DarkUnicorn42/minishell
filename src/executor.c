/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtcza <mwojtcza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:04:10 by mwojtcza          #+#    #+#             */
/*   Updated: 2024/10/03 15:04:12 by mwojtcza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int execute_commands(t_command *commands, t_shell *shell, t_history *history)
{
    t_command *cmd;
    int input_fd = STDIN_FILENO;
    pid_t last_pid = -1;
    int pipe_fd[2];

    cmd = commands;
    while (cmd)
    {
        if (is_builtin_parent(cmd->args[0]) && !cmd->next)
            shell->exit_code = execute_builtin(cmd, shell, history);
        else
        {
            if (cmd->next && create_pipe(pipe_fd) == -1)
                return (set_exit_code(shell, 1));

            pid_t pid = fork_process();
            if (pid == -1)
                return (set_exit_code(shell, 1));

            if (pid == 0)
            {
                setup_child_io(cmd, input_fd, pipe_fd);
                if (handle_redirections(cmd) == -1)
				{
                    exit(1);
				}
                if (is_builtin(cmd->args[0]))
                    shell->exit_code = execute_builtin(cmd, shell, history);
                else
                    execute_external(cmd, shell);
                exit(shell->exit_code);
            }
            else 
            {
                if (input_fd != STDIN_FILENO)
                    close(input_fd);
                if (cmd->next)
                {
                    close(pipe_fd[1]);
                    input_fd = pipe_fd[0];
                }
                last_pid = pid;
            }
        }
        cmd = cmd->next;
    }
    wait_for_children(shell, last_pid);
    return 0;
}

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
            command->args[i] = expand_argument(command->args[i], shell);
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
		return (builtin_exit(command->args, shell));
	else if (ft_strcmp(command->args[0], "history") == 0)
		return (builtin_history(history));
	return (0);
}

void execute_external(t_command *command, t_shell *shell)
{
    char *full_path;

    if (ft_strchr(command->args[0], '/'))
    {
        // Command contains '/', attempt to execute it directly
        full_path = ft_strdup(command->args[0]);
        if (!full_path)
        {
            ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
            exit(1);
        }
        if (access(full_path, F_OK) != 0)
        {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(full_path, STDERR_FILENO);
            ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
            free(full_path);
            exit(127);
        }
        else if (access(full_path, X_OK) != 0)
        {
            ft_putstr_fd("minishell: ", STDERR_FILENO);
            ft_putstr_fd(full_path, STDERR_FILENO);
            ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
            free(full_path);
            exit(126);
        }
    }
    else
    {
        // Command does not contain '/', search in PATH
        char *path_env = get_env_value("PATH", shell->envp);
        char **paths = ft_split(path_env, ':');
        full_path = find_executable_path(paths, command->args[0]);
        free_string_array(paths);
        if (!full_path)
        {
           // ft_putstr_fd(command->args[0], STDERR_FILENO);
           // ft_putstr_fd(": command not found\n", STDERR_FILENO);
            exit(127);
        }
    }

    // Attempt to execute the command
    execve(full_path, command->args, shell->envp);

    // If execve returns, an error occurred
    ft_putstr_fd("minishell: ", STDERR_FILENO);
    ft_putstr_fd(full_path, STDERR_FILENO);
    ft_putstr_fd(": ", STDERR_FILENO);
    ft_putstr_fd(strerror(errno), STDERR_FILENO);
    ft_putstr_fd("\n", STDERR_FILENO);

    free(full_path);

    exit(1);
}

char	*find_executable_path(char **paths, char *cmd)
{
	char	*full_path;
	char	*temp_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		temp_path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(temp_path, cmd);
		free(temp_path);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		else if (access(full_path, F_OK) == 0)
		{
			ft_putstr_fd(cmd, STDERR_FILENO);
			ft_putstr_fd(": permission denied\n", STDERR_FILENO);
			free(full_path);
			return (NULL);
		}
		free(full_path);
		i++;
	}
	return (NULL);
}
