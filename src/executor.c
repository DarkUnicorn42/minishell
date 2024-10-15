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

int execute_commands(t_command *commands, t_shell *shell, t_history *history) {
    t_command *current_command = commands;
    int pipe_fd[2];
    int input_fd = STDIN_FILENO;
    pid_t pid;
    int status;
    pid_t last_pid = -1; // To keep track of the last command's process ID

    while (current_command) {
        if (is_builtin(current_command->args[0])) {
            shell->exit_code = execute_builtin(current_command, shell, history);
        } else {
            if (current_command->next) {
                if (pipe(pipe_fd) == -1) {
                    perror("pipe");
                    shell->exit_code = 1;
                    return 1;
                }
            }

            pid = fork_process();
            if (pid == -1) {
                perror("fork");
                shell->exit_code = 1;
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
                if (handle_redirections(current_command) == -1) {
                    exit(1);
                }
                execvp(current_command->args[0], current_command->args);
                perror("execvp");
                exit(1);
            } else {
                // Parent process
                last_pid = pid; // Store the last child process ID
                if (input_fd != STDIN_FILENO) {
                    close(input_fd);
                }
                if (current_command->next) {
                    close(pipe_fd[1]);
                    input_fd = pipe_fd[0];
                }
            }
        }
        current_command = current_command->next;
    }

    // Wait for all child processes and update the exit code
    while ((pid = wait(&status)) > 0) {
        if (pid == last_pid && WIFEXITED(status)) {
            shell->exit_code = WEXITSTATUS(status);
        }
    }

    return 0;
}


int handle_redirections(t_command *command) {
    t_redirection *redir = command->redirections;
    int fd;

    while (redir) {
        if (redir->type == TOKEN_REDIRECT_IN) {
            fd = open(redir->file, O_RDONLY);
            if (fd == -1) {
                perror("open");
                return (-1);
            }
            dup2(fd, STDIN_FILENO);
        } else if (redir->type == TOKEN_REDIRECT_OUT) {
            fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1) {
                perror("open");
                return (-1);
            }
            dup2(fd, STDOUT_FILENO);
        } else if (redir->type == TOKEN_APPEND) {
            fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1) {
                perror("open");
                return (-1);
            }
            dup2(fd, STDOUT_FILENO);
        } else if (redir->type == TOKEN_HEREDOC) {
            // Handle heredoc (<<)
            // << should be given a delimiter, then read the input until a line containing the delimiter is seen.
        }

        close(fd);
        redir = redir->next;
    }
    return (0);
}

int is_builtin(char *cmd) {
    return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") ||
            !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export") ||
            !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env") ||
            !ft_strcmp(cmd, "exit") || !ft_strcmp(cmd, "history"));
}

int execute_builtin(t_command *command, t_shell *shell, t_history *history) {
    int i = 0;
    while (command->args[i]) {
        // Expand each argument of the command using expand_variables
        char *expanded_arg = expand_variables(command->args[i], shell);
        if (!expanded_arg) {
            perror("malloc");
            return 1;
        }
        free(command->args[i]); // Free the original argument
        command->args[i] = expanded_arg; // Replace it with the expanded version
        i++;
    }

    if (ft_strcmp(command->args[0], "echo") == 0)
        return builtin_echo(command->args);
    else if (ft_strcmp(command->args[0], "cd") == 0)
        return builtin_cd(command->args, shell);
    else if (ft_strcmp(command->args[0], "pwd") == 0)
        return builtin_pwd();
    else if (ft_strcmp(command->args[0], "export") == 0)
        return builtin_export(command->args, shell);
    else if (ft_strcmp(command->args[0], "unset") == 0)
        return builtin_unset(command->args);
    else if (ft_strcmp(command->args[0], "env") == 0)
        return builtin_env();
    else if (ft_strcmp(command->args[0], "exit") == 0)
        return builtin_exit(command->args);
    else if (ft_strcmp(command->args[0], "history") == 0)
        return builtin_history(history);

    return 0;
}