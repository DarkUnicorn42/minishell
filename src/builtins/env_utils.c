
#include "../../include/minishell.h"

char	**realloc_envp(char **envp, int new_size)
{
	char	**new_envp;
	int		i;

	new_envp = malloc(sizeof(char *) * new_size);
	if (!new_envp)
	{
		ft_putstr_fd("Error: malloc failed in realloc_envp\n", STDERR_FILENO);
		return (NULL);
	}
	i = 0;
	while (envp[i])
	{
		new_envp[i] = envp[i];
		i++;
	}
	return (new_envp);
}

char	**unset_env_value(const char *key, char **envp)
{
	int		key_len;
	int		i;
	int		j;

	key_len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, key_len) == 0 && envp[i][key_len] == '=')
		{
			free(envp[i]);
			j = i;
			while (envp[j])
			{
				envp[j] = envp[j + 1];
				j++;
			}
			return (envp);
		}
		i++;
	}
	return (envp);
}

char	**duplicate_envp(char **envp)
{
	int		count;
	char	**new_envp;

	count = 0;
	while (envp[count])
		count++;
	new_envp = malloc((count + 1) * sizeof(char *));
	if (!new_envp)
		return (NULL);
	if (!copy_envp(envp, new_envp, count))
		return (NULL);
	new_envp[count] = NULL;
	return (new_envp);
}

int	copy_envp(char **envp, char **new_envp, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
		{
			while (--i >= 0)
				free(new_envp[i]);
			free(new_envp);
			return (0);
		}
		i++;
	}
	return (1);
}
