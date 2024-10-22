
#include "../include/minishell.h"


int	handle_redirections(t_command *command)
{
	t_redirection	*redir;
	int				fd;

	redir = command->redirections;
	while (redir)
	{
		fd = open_file_for_redirection(redir);
		if (fd == -1)
			return (-1);
		dup2(fd, get_dup_fd(redir->type));
		close(fd);
		redir = redir->next;
	}
	return (0);
}

int	get_dup_fd(t_token_type type)
{
	if (type == TOKEN_REDIRECT_IN)
		return (STDIN_FILENO);
	return (STDOUT_FILENO);
}

int	open_file_for_redirection(t_redirection *redir)
{
	int	fd;

	if (redir->type == TOKEN_REDIRECT_IN)
		fd = open(redir->file, O_RDONLY);
	else if (redir->type == TOKEN_REDIRECT_OUT)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == TOKEN_APPEND)
		fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		return (-1); // Handle TOKEN_HEREDOC if implemented
	if (fd == -1)
	{
		ft_putstr_fd("Error: failed to open file\n", STDERR_FILENO);
		return (-1);
	}
	return (fd);
}
