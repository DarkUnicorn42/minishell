
#include "../include/minishell.h"

char	*expand_exit_code(t_shell *shell, int *i)
{
	char	*expansion;

	expansion = ft_itoa(shell->exit_code);
	if (!expansion)
		perror("malloc");
	*i += 2;
	return (expansion);
}

char	*expand_variable(const char *str, int *i)
{
	int		var_start;
	char	*var_name;
	char	*expansion;

	var_start = *i + 1;
	while (str[*i + 1] && (ft_isalnum(str[*i + 1]) || str[*i + 1] == '_'))
		(*i)++;
	var_name = ft_substr(str, var_start, *i + 1 - var_start);
	if (!var_name)
	{
		perror("malloc");
		return (NULL);
	}
	expansion = getenv(var_name);
	free(var_name);
	if (!expansion)
		expansion = "";
	(*i)++;
	return (ft_strdup(expansion));
}

char	*append_char(char *result, char c)
{
	char	temp[2];
	char	*new_result;

	temp[0] = c;
	temp[1] = '\0';
	new_result = ft_strjoin(result, temp);
	free(result);
	if (!new_result)
		perror("malloc");
	return (new_result);
}

char	*expand_variables(const char *str, t_shell *shell)
{
	char	*result;
	char	*expansion;
	char	*temp_result;
	int		i;

	result = ft_strdup("");
	if (!result)
	{
		perror("malloc");
		return (NULL);
	}
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			if (str[i + 1] == '?')
				expansion = expand_exit_code(shell, &i);
			else if (ft_isalpha(str[i + 1]) || str[i + 1] == '_')
				expansion = expand_variable(str, &i);
			else
			{
				expansion = ft_strdup("$");
				i++;
			}
			if (!expansion)
			{
				free(result);
				return (NULL);
			}
			temp_result = ft_strjoin(result, expansion);
			free(result);
			free(expansion);
			if (!temp_result)
			{
				perror("malloc");
				return (NULL);
			}
			result = temp_result;
		}
		else
		{
			result = append_char(result, str[i]);
			if (!result)
				return (NULL);
			i++;
		}
	}
	return (result);
}
