
#include "../../include/minishell.h"

void	print_exit_error(char *arg, char *message)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(message, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

int	print_error(char *message, int exit_code)
{
	ft_putstr_fd(message, STDERR_FILENO);
	return (exit_code);
}

void	print_tokens(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current != NULL)
	{
		printf("Token: Type = %d, Value = %s\n", current->type, current->value);
		current = current->next;
	}
}

int	set_exit_code(t_shell *shell, int code)
{
	shell->exit_code = code;
	return (code);
}

char	*expand_exit_code(t_shell *shell, int *i)
{
	*i += 2;
	return (ft_itoa(shell->exit_code));
}
