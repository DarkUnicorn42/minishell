/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtcza <mwojtcza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:04:10 by mwojtcza          #+#    #+#             */
/*   Updated: 2024/11/10 19:12:13 by mwojtcza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	execute_commands(t_command *commands, t_shell *shell, t_history *history)
{
	t_command	*cmd;

	cmd = commands;
	shell->input_fd = STDIN_FILENO;
	shell->last_pid = -1;
	while (cmd)
	{
		if (is_builtin_parent(cmd->args[0]) && !cmd->next)
			shell->exit_code = execute_builtin(cmd, shell, history);
		else
		{
			if (!execute_command(cmd, shell, history))
			{
				shell->commands = NULL;
				return (set_exit_code(shell, 1));
			}
		}
		cmd = cmd->next;
	}
	wait_for_children(shell);
	free_commands(commands);
	shell->commands = NULL;
	return (0);
}

int	execute_command(t_command *cmd, t_shell *shell, t_history *history)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (cmd->next && create_pipe(pipe_fd) == -1)
		return (0);
	pid = fork_process();
	if (pid == -1)
		return (0);
	if (pid == 0)
		execute_child(cmd, shell, history, pipe_fd);
	else
		execute_parent(cmd, pipe_fd, pid, shell);
	return (1);
}

void	execute_child(t_command *cmd, t_shell *shell, t_history *history
			, int pipe_fd[2])
{
	setup_child_io(cmd, shell->input_fd, pipe_fd);
	if (handle_redirections(cmd, shell) == -1)
		exit(shell->exit_code);
	if (is_builtin(cmd->args[0]))
		shell->exit_code = execute_builtin(cmd, shell, history);
	else
	{
		execute_external(cmd, shell);
	}
	cleanup_shell(shell, history);
	exit(shell->exit_code);
}

void	execute_parent(t_command *cmd, int pipe_fd[2], pid_t pid
			, t_shell *shell)
{
	if (shell->input_fd != STDIN_FILENO)
		close(shell->input_fd);
	if (cmd->next)
	{
		close(pipe_fd[1]);
		shell->input_fd = pipe_fd[0];
	}
	shell->last_pid = pid;
}

void	execute_external(t_command *command, t_shell *shell)
{
	char	*full_path;

	full_path = get_full_path(command, shell);
	if (!full_path)
	{
		shell->exit_code = 127;
		exit(shell->exit_code);
	}
	execute_command_full_path(full_path, command->args, shell->envp);
	free(full_path);
}
