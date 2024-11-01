
#include "../../include/minishell.h"

char	*get_full_path(t_command *command, t_shell *shell)
{
	char	*full_path;

	if (ft_strchr(command->args[0], '/'))
	{
		full_path = ft_strdup(command->args[0]);
		if (!full_path)
			print_error("minishell: memory allocation error\n", 1);
		if (!check_file_access(full_path))
		{
            free(full_path);
            return (NULL);
        }
	}
	else
	{
		full_path = search_in_path(command->args[0], shell->envp);
		if (!full_path)
			return (NULL);
	}
	return (full_path);
}

char	*find_executable_path(char **paths, char *cmd)
{
	int		i;
	char	*full_path;

	i = 0;
	while (paths[i])
	{
		full_path = join_path_cmd(paths[i], cmd);
		if (check_executable(full_path, cmd))
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*search_in_path(char *cmd, char **envp)
{
	char	*path_env;
	char	**paths;
	char	*full_path;

	path_env = get_env_value("PATH", envp);
	paths = ft_split(path_env, ':');
	full_path = find_executable_path(paths, cmd);
	free_string_array(paths);
	if (!full_path)
	{
		print_exit_error(cmd, ": command not found\n");
		//free(cmd);
	}
	return (full_path);
}
