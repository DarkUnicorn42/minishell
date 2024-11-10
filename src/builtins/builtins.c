/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtcza <mwojtcza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:03:59 by mwojtcza          #+#    #+#             */
/*   Updated: 2024/11/10 18:28:16 by mwojtcza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_echo(char **args)
{
	int	i;
	int	newline;	

	i = 1;
	newline = 1;
	if (args[i] && ft_strcmp(args[i], "-n") == 0)
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}

int	builtin_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		return (0);
	}
	else
	{
		perror("pwd");
		return (1);
	}
}

int	builtin_unset(char **args, t_shell *shell)
{
	int	i;
	int	exit_code;

	i = 1;
	exit_code = 0;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			fprintf(stderr, "unset: '%s': not a valid identifier\n", args[i]);
			exit_code = 1;
		}
		else
			shell->envp = unset_env_value(args[i], shell->envp);
		i++;
	}
	return (exit_code);
}

int	builtin_exit(char **args, t_shell *shell, t_history *history)
{
	int	exit_code;

	if (!args[1])
	{
		exit_code = shell->exit_code;
		cleanup_shell(shell, history);
		exit(exit_code);
	}
	if (!is_numeric(args[1]))
	{
		print_exit_error(args[1], "numeric argument required");
		cleanup_shell(shell, history);
		exit(2);
	}
	if (args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		shell->exit_code = 1;
		return (1);
	}
	exit_code = (unsigned char)ft_atoi(args[1]);
	cleanup_shell(shell, history);
	exit(exit_code);
}

int	builtin_history(t_history *history)
{
	int	i;

	i = 0;
	while (i < history->count)
	{
		printf("%d %s\n", i + 1, history->commands[i]);
		i++;
	}
	return (0);
}
