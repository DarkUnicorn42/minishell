/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtcza <mwojtcza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 14:07:36 by mwojtcza          #+#    #+#             */
/*   Updated: 2024/06/27 14:07:36 by mwojtcza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex/pipex.h"

void	error_exit(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

char	*get_path_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

char	*construct_command_path(const char *dir, const char *command)
{
	char	*command_path;
	size_t	dir_len;
	size_t	cmd_len;

	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(command);
	command_path = malloc(dir_len + cmd_len + 2);
	if (!command_path)
		error_exit("err_malloc");
	ft_strcpy(command_path, dir);
	command_path[dir_len] = '/';
	ft_strcpy(command_path + dir_len + 1, command);
	return (command_path);
}

char	*find_command_path(const char *command, char **envp)
{
	char	*path_env;
	char	**paths;
	char	*command_path;
	int		i;

	path_env = get_path_env(envp);
	if (!path_env)
		error_exit("err_PATH_not_found");
	paths = ft_split(path_env, ':');
	if (!paths)
		error_exit("err_split_PATH");
	i = 0;
	while (paths[i])
	{
		command_path = construct_command_path(paths[i], command);
		if (access(command_path, X_OK) == 0)
		{
			free(paths);
			return (command_path);
		}
		free(command_path);
		i++;
	}
	free(paths);
	return (NULL);
}

void	execute_cmd(char *cmd, char **envp)
{
	char	**args;
	char	*full_cmd;

	full_cmd = NULL;
	args = ft_split(cmd, ' ');
	if (!args)
		error_exit("err_ft_split");
	full_cmd = find_command_path(args[0], envp);
	if (!full_cmd)
		error_exit("err_cmd_not_found");
	if (execve(full_cmd, args, envp) == -1)
		error_exit("err_execve");
}
