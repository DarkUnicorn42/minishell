/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtcza <mwojtcza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 14:07:04 by mwojtcza          #+#    #+#             */
/*   Updated: 2024/06/27 14:07:04 by mwojtcza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <string.h>
# include <errno.h>
# include "../libft/libft.h"

void	error_exit(const char *message);
void	execute_cmd(char *cmd, char **envp);
void	pipex(int fd_in, int fd_out, char **argv, char **envp);
char	*get_path_env(char *envp[]);
char	*find_command_path(const char *command, char **envp);
void	first_child(int fd_in, int *pipe_fd, char **argv, char **envp);
void	second_child(int fd_out, int *pipe_fd, char **argv, char **envp);

#endif