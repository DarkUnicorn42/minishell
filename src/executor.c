/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwojtcza <mwojtcza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:04:10 by mwojtcza          #+#    #+#             */
/*   Updated: 2024/10/03 15:04:12 by mwojtcza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int execute_commands(t_command *commands, t_shell *shell) {
    t_command *current_command = commands;
    int pipe_fd[2];
    int input_fd = STDIN_FILENO;
    pid_t pid;

    while (current_command) {
        // Check if the current command is a builtin
        if (is_builtin(current_command->args[0])) {
            // Execute the builtin directly in the main process
            execute_builtin(current_command, shell);
        } else {
            // If there's a next command, create a pipe
            if (current_command->next) {
                if (pipe(pipe_fd) == -1) {
                    perror("pipe");
                    return 1;
                }
            }

            // Fork to execute the command
            pid = fork();
            if (pid == -1) {
                perror("fork");
                return 1;
            }
            if (pid == 0) {
                // Child process
                if (input_fd != STDIN_FILENO) {
                    dup2(input_fd, STDIN_FILENO);
                    close(input_fd);
                }
                if (current_command->next) {
                    close(pipe_fd[0]);
                    dup2(pipe_fd[1], STDOUT_FILENO);
                    close(pipe_fd[1]);
                }
                if (handle_redirections(current_command) == -1)
                    exit(1);
                execvp(current_command->args[0], current_command->args);
                perror("execvp");
                exit(1);
            } else {
                // Parent process
                if (input_fd != STDIN_FILENO)
                    close(input_fd);

                if (current_command->next) {
                    close(pipe_fd[1]);
                    input_fd = pipe_fd[0];
                }
                waitpid(pid, NULL, 0);
            }
        }
        current_command = current_command->next;
    }
    return 0;
}


int handle_redirections(t_command *command)
{
    t_redirection *redir = command->redirections;
    int fd;

    while (redir)
    {
        if (redir->type == TOKEN_REDIRECT_IN)
        {
            fd = open(redir->file, O_RDONLY);
            if (fd == -1)
            {
                perror("open");
                return (-1);
            }
            dup2(fd, STDIN_FILENO);
        }
        else if (redir->type == TOKEN_REDIRECT_OUT)
        {
            fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1)
            {
                perror("open");
                return (-1);
            }
            dup2(fd, STDOUT_FILENO);
        }
        else if (redir->type == TOKEN_APPEND)
        {
            fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1)
            {
                perror("open");
                return (-1);
            }
            dup2(fd, STDOUT_FILENO);
        }
        else if (redir->type == TOKEN_HEREDOC)
        {
            // Handle heredoc (<<)
            // << should be given a delimiter, then read the input until a line containing the delimiter is seen. 
            // However, it doesn’t have to update the history!
        }

        close(fd);
        redir = redir->next;
    }
    return (0);
}

int is_builtin(char *cmd)
{
    return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") ||
            !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export") ||
            !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env") ||
            !ft_strcmp(cmd, "exit"));
}

int execute_builtin(t_command *command, t_shell *shell)
{
    if (!ft_strcmp(command->args[0], "echo"))
        return builtin_echo(command->args);
    else if (!ft_strcmp(command->args[0], "cd"))
        return builtin_cd(command->args, shell);
    else if (!ft_strcmp(command->args[0], "pwd"))
        return builtin_pwd();
    else if (!ft_strcmp(command->args[0], "export"))
        return builtin_export(command->args, shell);
    else if (!ft_strcmp(command->args[0], "unset"))
        return builtin_unset(command->args);
    else if (!ft_strcmp(command->args[0], "env"))
        return builtin_env();
    else if (!ft_strcmp(command->args[0], "exit"))
        return builtin_exit(command->args);
    return 0;
}

