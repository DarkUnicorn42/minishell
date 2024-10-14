

#include "../include/minishell.h"

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