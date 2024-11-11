
#include "../../include/minishell.h"

void	free_envp(char ***envp)
{
	int	i;

	if (*envp)
	{
		i = 0;
		while ((*envp)[i])
		{
			free((*envp)[i]);
			i++;
		}
		free(*envp);
		*envp = NULL;
	}
}

void	free_current_dir(char **current_dir)
{
	if (*current_dir)
	{
		free(*current_dir);
		*current_dir = NULL;
	}
}

void	cleanup_shell(t_shell *shell, t_history *history)
{
	free_history(history);
	free_envp(&(shell->envp));
	free_current_dir(&(shell->current_dir));
	clear_shell_state(shell);
}

void	clear_shell_state(t_shell *shell)
{
	if (shell->tokens)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
	}
	if (shell->commands)
	{
		free_commands(shell->commands);
		shell->commands = NULL;
	}
}

void	initialize_shell(t_shell *shell)
{
	shell->tokens = NULL;
	shell->commands = NULL;
	shell->current_dir = NULL;
}
