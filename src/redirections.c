/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtcza <mwojtcza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 18:29:19 by mwojtcza          #+#    #+#             */
/*   Updated: 2024/11/10 18:29:19 by mwojtcza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int handle_redirections(t_command *command, t_shell *shell)
{
    t_redirection *redir;
    int fd;

    redir = command->redirections;
    while (redir)
    {
        fd = open_file_for_redirection(redir);
        if (fd == -1)
        {
            print_exit_error(redir->file, strerror(errno));
            shell->exit_code = 1;
            return (-1);
        }
        if (dup2(fd, get_dup_fd(redir->type)) == -1)
        {
            ft_putstr_fd("minishell: dup2 error\n", STDERR_FILENO);
            close(fd);
            shell->exit_code = 1;
            return (-1);
        }
        close(fd);
        redir = redir->next;
    }
    return (0);
}

int	get_dup_fd(t_token_type type)
{
	if (type == TOKEN_REDIRECT_IN || type == TOKEN_HEREDOC)
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
	else if (redir->type == TOKEN_HEREDOC)
		fd = handle_heredoc(redir->file);
	else
		return (-1);
	if (fd == -1)
		return (-1);
	return (fd);
}

int	handle_heredoc(char *delimiter)
{
	int		pipe_fd[2];
	char	*line;

	if (pipe(pipe_fd) == -1)
	{
		ft_putstr_fd("Error: pipe failed\n", STDERR_FILENO);
		return (-1);
	}
	while (1)
	{
		ft_putstr_fd("> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0 &&
			line[ft_strlen(delimiter)] == '\n')
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, pipe_fd[1]);
		free(line);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

void	free_redirections(t_redirection *redir)
{
	t_redirection	*next_redir;

	while (redir)
	{
		next_redir = redir->next;
		if (redir->file)
			free(redir->file);
		free(redir);
		redir = next_redir;
	}
}
