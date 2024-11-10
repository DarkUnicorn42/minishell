/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtcza <mwojtcza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 18:29:58 by mwojtcza          #+#    #+#             */
/*   Updated: 2024/11/10 18:29:58 by mwojtcza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_cd(char **args, t_shell *shell)
{
	char	*path;
	int		arg_count;

	arg_count = count_args(args);
	if (arg_count > 2)
		return (print_error("cd: too many arguments\n", 1));
	path = get_cd_path(args, shell);
	if (!path)
		return (1);
	if (chdir(path) == -1)
		return (print_error(" No such file or directory\n", 1));
	if (!update_pwd(shell))
		return (1);
	return (0);
}

char	*get_cd_path(char **args, t_shell *shell)
{
	char	*path;

	if (!args[1])
	{
		path = get_env_value("HOME", shell->envp);
		if (!path)
		{
			ft_putstr_fd("cd: HOME not set\n", STDERR_FILENO);
			return (NULL);
		}
	}
	else
		path = args[1];
	return (path);
}

int	update_pwd(t_shell *shell)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		shell->envp = set_env_val("PWD", cwd, shell->envp);
		if (!shell->envp)
			return (print_error("Error updating PWD\n", 1));
	}
	else
		return (print_error("getcwd failed\n", 0));
	return (1);
}

int	count_args(char **args)
{
	int	count;

	count = 0;
	while (args[count])
		count++;
	return (count);
}