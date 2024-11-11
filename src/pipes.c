/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtcza <mwojtcza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 18:29:12 by mwojtcza          #+#    #+#             */
/*   Updated: 2024/11/10 18:29:12 by mwojtcza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	create_pipe(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
	{
		ft_putstr_fd("Error: pipe failed\n", STDERR_FILENO);
		return (-1);
	}
	return (0);
}

pid_t	fork_process(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		ft_putstr_fd("Error: fork failed\n", STDERR_FILENO);
	return (pid);
}

int	parent_process(int input_fd, int pipe_fd[2], t_command *cmd)
{
	if (input_fd != STDIN_FILENO)
		close(input_fd);
	if (cmd->next)
	{
		close(pipe_fd[1]);
		return (pipe_fd[0]);
	}
	return (STDIN_FILENO);
}

void wait_for_children(t_shell *shell)
{
	int		status;
	pid_t	pid;

	while ((pid = wait(&status)) > 0)
	{
		if (pid == shell->last_pid)
		{
			if (WIFEXITED(status))
				shell->exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				shell->exit_code = 128 + WTERMSIG(status);
			else
				shell->exit_code = 1;
		}
	}
}

void	setup_child_io(t_command *cmd, int input_fd, int pipe_fd[2])
{
	if (input_fd != STDIN_FILENO)
	{
		dup2(input_fd, STDIN_FILENO);
		close(input_fd);
	}
	if (cmd->next)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
}
