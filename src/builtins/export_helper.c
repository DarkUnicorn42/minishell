/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtcza <mwojtcza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 19:10:41 by mwojtcza          #+#    #+#             */
/*   Updated: 2024/11/10 19:11:03 by mwojtcza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	print_export_env(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->envp[i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putendl_fd(shell->envp[i], STDOUT_FILENO);
		i++;
	}
	return (0);
}

int	parse_export_arg(char *arg, char **key, char **value)
{
	char	*equal_sign;

	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		*key = ft_substr(arg, 0, equal_sign - arg);
		*value = ft_strdup(equal_sign + 1);
	}
	else
	{
		*key = ft_strdup(arg);
		*value = NULL;
	}
	if (!*key || (equal_sign && !*value))
		return (-1);
	if (!is_valid_identifier(*key))
		return (0);
	return (1);
}

int	print_export_id_error(char *identifier)
{
	ft_putstr_fd("export: `", STDERR_FILENO);
	ft_putstr_fd(identifier, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	return (0);
}
