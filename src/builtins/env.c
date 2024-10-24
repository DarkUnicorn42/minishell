

#include "../../include/minishell.h"

int	builtin_env(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->envp[i])
	{
		ft_putstr_fd(shell->envp[i], STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		i++;
	}
	return (0);
}

char	*get_env_value(const char *name, char **envp)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(name);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (&envp[i][len + 1]);
		i++;
	}
	return (NULL);
}

char	*construct_env_entry(const char *key, const char *value)
{
	char	*entry;
	char	*tmp;

	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (NULL);
	entry = ft_strjoin(tmp, value);
	free(tmp);
	if (!entry)
		return (NULL);
	return (entry);
}

char	**set_env_value(const char *key, const char *value, char **envp)
{
	int		key_len;
	char	*new_entry;
	int		i;

	key_len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, key_len) == 0 && envp[i][key_len] == '=')
		{
			free(envp[i]);
			new_entry = construct_env_entry(key, value);
			if (!new_entry)
			{
				ft_putstr_fd("Error: malloc failed in set_env_value\n", STDERR_FILENO);
				return (envp);
			}
			envp[i] = new_entry;
			return (envp);
		}
		i++;
	}
	return (add_env_entry(envp, key, value));
}

char	**add_env_entry(char **envp, const char *key, const char *value)
{
	char	**new_envp;
	int		i;
	char	*new_entry;

	i = 0;
	while (envp[i])
		i++;
	new_envp = malloc(sizeof(char *) * (i + 2));
	if (!new_envp)
	{
		ft_putstr_fd("Error: malloc failed in add_env_entry\n", STDERR_FILENO);
		return (envp);
	}
	i = -1;
	while (envp[++i])
		new_envp[i] = envp[i];
	new_entry = construct_env_entry(key, value);
	if (!new_entry)
	{
		ft_putstr_fd("Error: malloc failed in add_env_entry\n", STDERR_FILENO);
		free(new_envp);
		return (envp);
	}
	new_envp[i++] = new_entry;
	new_envp[i] = NULL;
	free(envp);
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
