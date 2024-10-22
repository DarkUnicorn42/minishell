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

int	execute_commands(t_command *commands, t_shell *shell, t_history *history)
{
	t_command	*cmd;
	int			input_fd;
	pid_t		pid;
	int			pipe_fd[2];

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
				input_fd = parent_process(input_fd, pipe_fd, cmd);
		}
		cmd = cmd->next;
	}
	wait_for_children(shell);
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

char	*expand_argument(char *arg, t_shell *shell)
{
	char	*expanded_arg;

	expanded_arg = expand_variables(arg, shell);
	if (!expanded_arg)
	{
		ft_putstr_fd("Error: malloc failed in expand_argument\n", STDERR_FILENO);
		return (arg);
	}
	free(arg);
	return (expanded_arg);
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
		return (builtin_exit(command->args));
	else if (ft_strcmp(command->args[0], "history") == 0)
		return (builtin_history(history));
	return (0);
}

void	execute_external(t_command *command, t_shell *shell)
{
	char	*path_env;
	char	**paths;
	char	*full_path;

	path_env = get_env_value("PATH", shell->envp);
	paths = ft_split(path_env, ':');
	full_path = find_executable_path(paths, command->args[0]);
	if (!full_path)
	{
		ft_putstr_fd(command->args[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		free_string_array(paths);
		exit(127);
	}
	execve(full_path, command->args, shell->envp);
	ft_putstr_fd("Error: execve failed\n", STDERR_FILENO);
	free(full_path);
	free_string_array(paths);
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
		free(full_path);
		i++;
	}
	return (NULL);
}

