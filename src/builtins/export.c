/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtcza <mwojtcza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 18:29:52 by mwojtcza          #+#    #+#             */
/*   Updated: 2024/11/10 18:29:52 by mwojtcza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_export(char **args, t_shell *shell)
{
	int		i;
	int		exit_code;

	if (!args[1])
		return (print_export_env(shell));
	exit_code = 0;
	i = 1;
	while (args[i])
	{
		if (!process_export_arg(args[i], shell))
			exit_code = 1;
		i++;
	}
	return (exit_code);
}

int	process_export_arg(char *arg, t_shell *shell)
{
	char	*key;
	char	*value;
	int		result;
	int		count;

	count = 0;
	result = parse_export_arg(arg, &key, &value);
	if (result == -1)
		return (print_error("malloc error\n", 1));
	if (result == 0)
		return (print_export_id_error(arg));
	if (update_envp(shell->envp, key, value) == -1)
	{
		if (expand_envp(shell, arg, count))
			return (print_error("malloc error\n", 1));
	}
	free(key);
	free(value);
	return (1);
}

int	expand_envp(t_shell *shell, char *new_var, int count)
{
	int		i;
	char	**new_envp;

	i = 0;
	while (shell->envp[count])
		count++;
	new_envp = malloc((count + 2) * sizeof(char *));
	if (!new_envp)
		return (1);
	while (i < count)
	{
		new_envp[i] = shell->envp[i];
		i++;
	}
	new_envp[count] = ft_strdup(new_var);
	if (!new_envp[count])
	{
		free(new_envp);
		return (1);
	}
	new_envp[count + 1] = NULL;
	free(shell->envp);
	shell->envp = new_envp;
	return (0);
}

char	**duplicate_envp_with_space(char **envp)
{
	int		count;
	int		i;
	char	**new_envp;

	count = 0;
	while (envp[count])
		count++;
	new_envp = malloc((count + 2) * sizeof(char *));
	if (!new_envp)
	{
		perror("malloc");
		return (NULL);
	}
	i = 0;
	while (i < count)
	{
		new_envp[i] = envp[i];
		i++;
	}
	return (new_envp);
}

int	update_envp(char **envp, char *key, char *new_value)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, ft_strlen(key)) == 0
			&& envp[i][ft_strlen(key)] == '=')
		{
			free(envp[i]);
			envp[i] = ft_strdup(new_value);
			if (!envp[i])
			{
				perror("malloc");
				return (1);
			}
			return (0);
		}
		i++;
	}
	return (-1);
}
