#include "../include/minishell.h"

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
	expansion = getenv(var_name) ? getenv(var_name) : "";
	free(var_name);
	(*i)++;
	return (ft_strdup(expansion));
}

char	*expand_exit_code(t_shell *shell, int *i)
{
	*i += 2;  // Skip "$?"
	return (ft_itoa(shell->exit_code));
}

char	*expand_variables(const char *str, t_shell *shell)
{
	char	*result = ft_strdup("");
	char	*expansion;
	int		i = 0;

	if (!result)
	{
		perror("malloc");
		return (NULL);
	}

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
		}
		else
		{
			expansion = ft_substr(str, i, 1);
			i++;
		}

		if (!expansion)
		{
			free(result);
			return (NULL);
		}

		char *temp = ft_strjoin(result, expansion);
		free(result);
		free(expansion);

		if (!temp)
		{
			perror("malloc");
			return (NULL);
		}

		result = temp;
	}

	return (result);
}
