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
    int input_fd;
    pid_t pid;
    int pipe_fd[2];
    pid_t last_pid = -1; // Variable to store the PID of the last child

    cmd = commands;
    input_fd = STDIN_FILENO;
    while (cmd)
    {
        if (is_builtin_parent(cmd->args[0]) && !cmd->next)
            shell->exit_code = execute_builtin(cmd, shell, history);
        else
        {
            if (cmd->next && create_pipe(pipe_fd) == -1)
                return (set_exit_code(shell, 1));
            pid = fork_process();
            if (pid == -1)
                return (set_exit_code(shell, 1));
            if (pid == 0)
                execute_child(cmd, shell, history, input_fd, pipe_fd);
            else
            {
                if (input_fd != STDIN_FILENO)
                    close(input_fd);
                if (cmd->next)
                {
                    close(pipe_fd[1]);
                    input_fd = pipe_fd[0];
                }
                else
                {
                    close(pipe_fd[0]);
                    close(pipe_fd[1]);
                }
                last_pid = pid; // Store the PID of the last child process
            }
        }
        cmd = cmd->next;
    }
    wait_for_children(shell, last_pid); // Pass last_pid to wait_for_children
    return (0);
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

int	execute_builtin(t_command *command, t_shell *shell, t_history *history)
{
	int	i;

	i = 0;
	while (command->args[i])
	{
		if (command->arg_types[i] == TOKEN_DOUBLE_QUOTED ||
			command->arg_types[i] == TOKEN_WORD)
			command->args[i] = expand_argument(command->args[i], shell);
		i++;
	}
	return (run_builtin_command(command, shell, history));
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

void execute_external(t_command *cmd, t_shell *shell) {
    if (cmd->args[0] == NULL || strcmp(cmd->args[0], "") == 0) {
        shell->exit_code = 0;
        return;
    }

    struct stat st;
    if (stat(cmd->args[0], &st) == 0 && S_ISDIR(st.st_mode)) {
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(cmd->args[0], STDERR_FILENO);
        ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
        shell->exit_code = 126;
        return;
    }

    if (access(cmd->args[0], X_OK) == -1) {
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(cmd->args[0], STDERR_FILENO);
        ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
        shell->exit_code = 126;
        return;
    }
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
		free(full_path);
		i++;
	}
	return (NULL);
}
