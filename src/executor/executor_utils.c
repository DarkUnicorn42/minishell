/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtcza <mwojtcza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 18:29:46 by mwojtcza          #+#    #+#             */
/*   Updated: 2024/11/10 18:29:46 by mwojtcza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*join_path_cmd(char *path, char *cmd)
{
	char	*temp_path;
	char	*full_path;

	temp_path = ft_strjoin(path, "/");
	full_path = ft_strjoin(temp_path, cmd);
	free(temp_path);
	return (full_path);
}

int	check_executable(char *full_path, char *cmd)
{
	if (access(full_path, X_OK) == 0)
		return (1);
	else if (access(full_path, F_OK) == 0)
	{
		print_exit_error(cmd, ": permission denied\n");
		return (0);
	}
	return (0);
}

void	execute_command_full_path(char *full_path, char **args, char **envp)
{
	execve(full_path, args, envp);
	print_exit_error(full_path, ": execve error\n");
	free(full_path);
	exit(1);
}

int	check_file_access(char *full_path)
{
	if (access(full_path, F_OK) != 0)
	{
		print_exit_error(full_path, ": No such file or directory\n");
		free(full_path);
		return (0);
	}
	else if (access(full_path, X_OK) != 0)
	{
		print_exit_error(full_path, ": Permission denied\n");
		free(full_path);
		return (0);
	}
	return (1);
}
