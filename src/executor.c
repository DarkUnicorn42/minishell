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
    int input_fd = STDIN_FILENO; // Input FD for the current command, initially stdin
    pid_t pid;
    int status;
    pid_t last_pid = -1;

    while (current_command) {
        // Create a pipe if there's a next command
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
            // Set up input redirection from the previous command
            if (input_fd != STDIN_FILENO) {
                if (dup2(input_fd, STDIN_FILENO) == -1) {
                    perror("dup2");
                    exit(1);
                }
                close(input_fd);
            }

            // Set up output redirection to the next command
            if (current_command->next) {
                close(pipe_fd[0]);  // Close read end of the pipe in the child
                if (dup2(pipe_fd[1], STDOUT_FILENO) == -1) {
                    perror("dup2");
                    exit(1);
                }
                close(pipe_fd[1]);
            }

            if (handle_redirections(current_command) == -1) {
                perror("handle_redirections");
                exit(1);
            }

            if (is_builtin(current_command->args[0])) {
                // Execute built-in in the child process context
                shell->exit_code = execute_builtin(current_command, shell, history);
                exit(shell->exit_code);  // Exit after executing the built-in
            } else {
                execvp(current_command->args[0], current_command->args);
                perror("execvp failed");
                exit(1);
            }
        } else {
            // Parent process
            last_pid = pid;

            if (input_fd != STDIN_FILENO) {
                close(input_fd);
            }

            // Update input_fd for the next command (read end of the pipe)
            if (current_command->next) {
                close(pipe_fd[1]);  // Close write end of the pipe in the parent
                input_fd = pipe_fd[0];  // Update input_fd for the next command in the pipeline
            }
        }

        current_command = current_command->next;
    }

    // Wait for all child processes and set the exit code
    while ((pid = wait(&status)) > 0) {
        if (pid == last_pid) {
            if (WIFEXITED(status)) {
                shell->exit_code = WEXITSTATUS(status);
            } else {
                shell->exit_code = 1;  // Handle unexpected termination
            }
        }
    }

    return 0;
}

int create_pipe(int pipe_fd[2]) {
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        return -1;
    }
    return 0;
}

pid_t fork_process(void) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
    }
    return pid;
}

void handle_child(int input_fd, int pipe_fd[2], t_command *current_command, t_shell *shell) {
    (void)shell;
    
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
        if (command->arg_types[i] == TOKEN_DOUBLE_QUOTED) {
            // Expand arguments if they are double-quoted
            // printf("Executor: Expanding double-quoted argument -> %s\n", command->args[i]);
            char *expanded_arg = expand_variables(command->args[i], shell);
            if (!expanded_arg) {
                perror("malloc");
                return 1;
            }
            free(command->args[i]);
            command->args[i] = expanded_arg;
        } else if (command->arg_types[i] == TOKEN_SINGLE_QUOTED) {
            // Single-quoted strings should not be expanded
            // printf("Executor: No expansion for single-quoted argument -> %s\n", command->args[i]);
        } else if (command->arg_types[i] == TOKEN_WORD) {
            // Expand arguments if they are unquoted
            // printf("Executor: Expanding unquoted argument -> %s\n", command->args[i]);
            char *expanded_arg = expand_variables(command->args[i], shell);
            if (!expanded_arg) {
                perror("malloc");
                return 1;
            }
            free(command->args[i]);
            command->args[i] = expanded_arg;
        } else {
            printf("Executor: Unknown token type for argument -> %s\n", command->args[i]);
        }
        i++;
    }

    // Debug statement for executing builtins
    // printf("Executor: Executing builtin command -> %s\n", command->args[0]);

    // Execute the appropriate builtin command
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
        return builtin_env(shell->envp);
    else if (ft_strcmp(command->args[0], "exit") == 0)
        return builtin_exit(command->args);
    else if (ft_strcmp(command->args[0], "history") == 0)
        return builtin_history(history);

    return 0;
}
