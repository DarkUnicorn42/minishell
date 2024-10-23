
#include "../include/minishell.h"

char	*expand_variable(const char *str, int *i, t_shell *shell)
{
	int		var_start;
	char	*var_name;
	char	*expansion;
	char	*env_value;

	var_start = *i + 1;
	while (str[*i + 1] && (ft_isalnum(str[*i + 1]) || str[*i + 1] == '_'))
		(*i)++;
	var_name = ft_substr(str, var_start, *i + 1 - var_start);
	if (!var_name)
	{
		ft_putstr_fd("Error: malloc failed in expand_variable\n", 2);
		return (NULL);
	}
	env_value = get_env_value(var_name, shell->envp);
	if (env_value)
		expansion = ft_strdup(env_value);
	else
		expansion = ft_strdup("");
	free(var_name);
	(*i)++;
	return (expansion);
}

char	*expand_exit_code(t_shell *shell, int *i)
{
	*i += 2;
	return (ft_itoa(shell->exit_code));
}

char	*expand_variables(const char *str, t_shell *shell)
{
	char	*result;
	int		i;

	i = 0;
	result = ft_strdup("");
	if (!result)
	{
		ft_putstr_fd("Error: malloc failed in expand_variables\n", 2);
		return (NULL);
	}
	while (str[i])
	{
		result = append_expanded_token(result, str, &i, shell);
		if (!result)
			return (NULL);
	}
	return (result);
}

char	*append_expanded_token(char *result, const char *str, int *i, t_shell *shell)
{
	char	*expansion;
	char	*temp;

	if (str[*i] == '$')
		expansion = get_expansion(str, i, shell);
	else
		expansion = ft_substr(str, (*i)++, 1);
	if (!expansion)
	{
		free(result);
		return (NULL);
	}
	temp = ft_strjoin(result, expansion);
	free(result);
	free(expansion);
	if (!temp)
	{
		ft_putstr_fd("Error: malloc failed in append_expanded_token\n", 2);
		return (NULL);
	}
	return (temp);
}

char	*get_expansion(const char *str, int *i, t_shell *shell)
{
	char	*expansion;

	if (str[*i + 1] == '?')
		expansion = expand_exit_code(shell, i);
	else if (ft_isalpha(str[*i + 1]) || str[*i + 1] == '_')
		expansion = expand_variable(str, i, shell);
	else
	{
		expansion = ft_strdup("$");
		(*i)++;
	}
	return (expansion);
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
	arg = NULL;
	return (expanded_arg);
}