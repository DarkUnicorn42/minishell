
## **Table of Contents**

1. [Overview](https://www.notion.so/Minishell-overview-12f74a5f45598024a67eca3dd8ec7ccf?pvs=21)
2. [Main Components](https://www.notion.so/Minishell-overview-12f74a5f45598024a67eca3dd8ec7ccf?pvs=21)
3. [Initialization and Main Loop](https://www.notion.so/Minishell-overview-12f74a5f45598024a67eca3dd8ec7ccf?pvs=21)
4. [Processing User Input](https://www.notion.so/Minishell-overview-12f74a5f45598024a67eca3dd8ec7ccf?pvs=21)
5. [Lexical Analysis (Lexer)](https://www.notion.so/Minishell-overview-12f74a5f45598024a67eca3dd8ec7ccf?pvs=21)
6. [Parsing Tokens into Commands](https://www.notion.so/Minishell-overview-12f74a5f45598024a67eca3dd8ec7ccf?pvs=21)
7. [Executing Commands](https://www.notion.so/Minishell-overview-12f74a5f45598024a67eca3dd8ec7ccf?pvs=21)
8. [Built-in Commands](https://www.notion.so/Minishell-overview-12f74a5f45598024a67eca3dd8ec7ccf?pvs=21)
9. [Environment Variable Handling](https://www.notion.so/Minishell-overview-12f74a5f45598024a67eca3dd8ec7ccf?pvs=21)
10. [Signal Handling](https://www.notion.so/Minishell-overview-12f74a5f45598024a67eca3dd8ec7ccf?pvs=21)
11. [Utilities and Helpers](https://www.notion.so/Minishell-overview-12f74a5f45598024a67eca3dd8ec7ccf?pvs=21)
12. [Cleanup and Memory Management](https://www.notion.so/Minishell-overview-12f74a5f45598024a67eca3dd8ec7ccf?pvs=21)
13. [Examples](https://www.notion.so/Minishell-overview-12f74a5f45598024a67eca3dd8ec7ccf?pvs=21)

---

## **1. Overview**

Your `minishell` is designed to:

- Read user input from the command line.
- Parse the input into tokens and commands.
- Handle built-in commands like `cd`, `echo`, `pwd`, `export`, `unset`, `env`, and `exit`.
- Execute external commands by searching in the system `PATH`.
- Manage environment variables.
- Handle redirections (`<`, `>`, `>>`, `<<`) and pipes (`|`).
- Maintain a command history.
- Handle signals like `Ctrl+C` and `Ctrl+D`.

---

## **2. Main Components**

The code is organized into several key components:

- **`minishell.c`**: Contains the main function and initialization routines.
- **Lexer (`lexer.c`)**: Tokenizes the user input.
- **Parser (`parser.c`)**: Parses tokens into command structures.
- **Executor (`executor.c`)**: Executes the parsed commands.
- **Built-ins (`builtins.c`)**: Implements built-in shell commands.
- **Environment Handling (`env` functions)**: Manages environment variables.
- **Signal Handling (`signals.c`)**: Handles interrupts like `Ctrl+C`.
- **Utilities (`utils.c`)**: Helper functions for various tasks.

---

## **3. Initialization and Main Loop**

### **`minishell.c`**

### **Initialization**

- **Function:** `init_shell`
    
    Initializes the shell environment and command history.
    
    ```c
    int init_shell(t_shell *shell, char **envp, t_history *history);
    
    ```
    
    - **Duplicates environment variables** into `shell->envp`.
    - **Sets up signal handlers** via `setup_signals()`.
    - **Allocates memory** for command history.

### **Main Loop**

- **Function:** `main`
    
    The main function runs an infinite loop where it:
    
    - **Prompts the user** for input using `readline`.
    - **Processes the input** by calling `process_input`.
    - **Handles `Ctrl+D` (EOF)** by checking if `input` is `NULL` and breaking the loop.
    - **Cleans up** resources before exiting.

---

## **4. Processing User Input**

### **Function:** `process_input`

```c
void process_input(char *input, t_shell *shell, t_history *history);

```

- **Adds input to history** if it's not empty.
- **Tokenizes the input** using the lexer (`lexer` function).
- **Parses tokens into commands** using the parser (`parse_tokens` function).
- **Executes commands** by calling `execute_commands`.
- **Frees allocated resources** after execution.

---

## **5. Lexical Analysis (Lexer)**

### **File:** `lexer.c`

The lexer converts the raw input string into a linked list of tokens.

### **Token Types**

Defined in `minishell.h`:

- `TOKEN_WORD`
- `TOKEN_PIPE`
- `TOKEN_REDIRECT_IN` (`<`)
- `TOKEN_REDIRECT_OUT` (`>`)
- `TOKEN_HEREDOC` (`<<`)
- `TOKEN_APPEND` (`>>`)
- `TOKEN_SINGLE_QUOTED`
- `TOKEN_DOUBLE_QUOTED`
- `TOKEN_EOF`

### **Function:** `lexer`

```c
t_token *lexer(const char *input);

```

- **Iterates over the input string**.
- **Skips whitespace**.
- **Identifies tokens**:
    - **Operators** (`|`, `<`, `>`, `<<`, `>>`).
    - **Words** (command names, arguments).
    - **Quoted strings** (`'single'` or `"double"`).
- **Creates tokens** using `create_token` and adds them to the token list.

### **Example**

**Input:** `echo "Hello, World!" > output.txt`

**Tokens Generated:**

1. `TOKEN_WORD` with value `echo`
2. `TOKEN_DOUBLE_QUOTED` with value `Hello, World!`
3. `TOKEN_REDIRECT_OUT`
4. `TOKEN_WORD` with value `output.txt`

---

## **6. Parsing Tokens into Commands**

### **File:** `parser.c`

Converts the list of tokens into a linked list of command structures (`t_command`).

### **Function:** `parse_tokens`

```c
t_command *parse_tokens(t_token *tokens);

```

- **Iterates over the tokens**.
- **Creates command structures** when necessary.
- **Adds arguments** to the command.
- **Handles redirections** and adds them to the command's redirection list.
- **Handles pipes** by starting a new command.

### **Command Structure (`t_command`)**

```c
typedef struct s_command {
    char **args;
    t_token_type *arg_types;
    t_redirection *redirections;
    struct s_command *next;
} t_command;

```

- **`args`**: Array of argument strings.
- **`arg_types`**: Types of arguments (e.g., quoted, unquoted).
- **`redirections`**: Linked list of redirection structures.
- **`next`**: Pointer to the next command (for pipelines).

### **Example**

**Tokens from Previous Example:**

1. `TOKEN_WORD` with value `echo`
2. `TOKEN_DOUBLE_QUOTED` with value `Hello, World!`
3. `TOKEN_REDIRECT_OUT`
4. `TOKEN_WORD` with value `output.txt`

**Parsing Result:**

- **Command:** `echo`
    - **Arguments:** `["echo", "Hello, World!"]`
    - **Redirections:**
        - Type: `TOKEN_REDIRECT_OUT`
        - File: `output.txt`

---

## **7. Executing Commands**

### **File:** `executor.c`

Executes the list of commands generated by the parser.

### **Function:** `execute_commands`

```c
int execute_commands(t_command *commands, t_shell *shell, t_history *history);

```

- **Iterates over the command list**.
- **Handles built-in commands** differently if they need to be executed in the parent process.
- **Forks processes** for external commands or built-in commands that can be executed in child processes.
- **Sets up pipes** if necessary.
- **Waits for child processes** to finish.

### **Command Execution Flow**

1. **Check if the command is a built-in**:
    - If it's a built-in like `cd` or `exit`, execute it in the parent process.
2. **Fork a child process** for external commands.
3. **In the child process**:
    - **Set up input/output redirections**.
    - **Execute the command** using `execve`.
4. **In the parent process**:
    - **Close unnecessary file descriptors**.
    - **Wait for child processes**.

### **Example**

**Command:** `ls -la | grep ".c" > files.txt`

- **First Command:** `ls -la`
    - **Piped to** the next command.
- **Second Command:** `grep ".c"`
    - **Redirects output** to `files.txt`.

---

## **8. Built-in Commands**

### **File:** `builtins.c`

Implements the built-in shell commands.

### **Available Built-ins**

- **`echo`**: Prints arguments to the standard output.
- **`cd`**: Changes the current directory.
- **`pwd`**: Prints the current working directory.
- **`export`**: Sets environment variables.
- **`unset`**: Unsets environment variables.
- **`env`**: Prints the environment variables.
- **`exit`**: Exits the shell.
- **`history`**: Shows the command history.

### **Function:** `execute_builtin`

```c
int execute_builtin(t_command *command, t_shell *shell, t_history *history);

```

- **Expands arguments** for variable substitution.
- **Calls the appropriate built-in function** based on the command name.

### **Example**

**Command:** `export PATH="/usr/local/bin:$PATH"`

- **Sets the `PATH` environment variable** by adding `/usr/local/bin` to it.

---

## **9. Environment Variable Handling**

### **Environment Functions**

- **`get_env_value`**: Retrieves the value of an environment variable.
- **`set_env_value`**: Sets or updates an environment variable.
- **`unset_env_value`**: Removes an environment variable.

### **Example**

**Function:** `get_env_value`

```c
char *get_env_value(const char *name, char **envp);

```

- **Searches the `envp` array** for the variable `name`.
- **Returns the value** after the `=` sign.

**Usage Example:**

```c
char *home_dir = get_env_value("HOME", shell->envp);

```

---

## **10. Signal Handling**

### **File:** `signals.c`

Handles signals like `Ctrl+C` (`SIGINT`) and `Ctrl+\` (`SIGQUIT`).

### **Function:** `setup_signals`

```c
void setup_signals(void);

```

- **Sets up signal handlers** for `SIGINT` and `SIGQUIT`.
- **Custom handlers**:
    - **`handle_sigint`**: Handles `Ctrl+C` by writing a newline and re-displaying the prompt.
    - **`ignore_sigquit`**: Ignores `SIGQUIT` (`Ctrl+\`).

---

## **11. Utilities and Helpers**

### **File:** `utils.c`

Contains various helper functions.

### **Memory Management**

- **`free_tokens`**: Frees the linked list of tokens.
- **`free_commands`**: Frees the linked list of commands.
- **`free_arguments`**: Frees the array of arguments.
- **`free_redirections`**: Frees the linked list of redirections.

### **String Operations**

- **`ft_strdup`**: Duplicates a string.
- **`ft_strjoin`**: Joins two strings.
- **`ft_substr`**: Extracts a substring.

### **Error Handling**

- **`print_error`**: Prints an error message to `stderr`.
- **`print_exit_error`**: Prints an error message and exits.

---

## **12. Cleanup and Memory Management**

### **Function:** `cleanup_shell`

```c
void cleanup_shell(t_shell *shell, t_history *history);

```

- **Frees all allocated memory** associated with `shell` and `history`.
- **Ensures no memory leaks** occur when the shell exits.

### **Example Usage in `builtin_exit`**

```c
int builtin_exit(char **args, t_shell *shell, t_history *history)
{
    cleanup_shell(shell, history);
    exit(shell->exit_code);
}

```

---

## **13. Examples**

Let's go through a few examples to see how the shell handles different commands.

### **Example 1: Simple Command**

**Input:** `echo Hello, World!`

**Process:**

1. **Lexing:**
    - Tokens:
        1. `TOKEN_WORD` with value `echo`
        2. `TOKEN_WORD` with value `Hello,`
        3. `TOKEN_WORD` with value `World!`
2. **Parsing:**
    - Command:
        - **Args:** `["echo", "Hello,", "World!"]`
        - **No redirections or pipes.**
3. **Execution:**
    - **Identifies `echo` as a built-in.**
    - **Executes `builtin_echo`.**
    - **Output:** `Hello, World!`

### **Example 2: Command with Redirection**

**Input:** `ls -l > output.txt`

**Process:**

1. **Lexing:**
    - Tokens:
        1. `TOKEN_WORD` with value `ls`
        2. `TOKEN_WORD` with value `l`
        3. `TOKEN_REDIRECT_OUT`
        4. `TOKEN_WORD` with value `output.txt`
2. **Parsing:**
    - Command:
        - **Args:** `["ls", "-l"]`
        - **Redirection:**
            - Type: `TOKEN_REDIRECT_OUT`
            - File: `output.txt`
3. **Execution:**
    - **Forks a child process.**
    - **Sets up output redirection** to `output.txt`.
    - **Executes `ls -l`.**
    - **Output is written** to `output.txt`.

### **Example 3: Piped Commands**

**Input:** `cat file.txt | grep "error" | sort`

**Process:**

1. **Lexing:**
    - Tokens:
        1. `TOKEN_WORD` with value `cat`
        2. `TOKEN_WORD` with value `file.txt`
        3. `TOKEN_PIPE`
        4. `TOKEN_WORD` with value `grep`
        5. `TOKEN_DOUBLE_QUOTED` with value `error`
        6. `TOKEN_PIPE`
        7. `TOKEN_WORD` with value `sort`
2. **Parsing:**
    - **First Command:**
        - **Args:** `["cat", "file.txt"]`
    - **Second Command:**
        - **Args:** `["grep", "error"]`
    - **Third Command:**
        - **Args:** `["sort"]`
3. **Execution:**
    - **Sets up pipes between commands.**
    - **Each command is executed in a child process.**
    - **Data flows through the pipeline:**
        - `cat file.txt` outputs the contents of `file.txt`.
        - `grep "error"` filters lines containing "error".
        - `sort` sorts the filtered lines.

### **Example 4: Built-in Command `export`**

**Input:** `export MY_VAR="Hello"`

**Process:**

1. **Lexing:**
    - Tokens:
        1. `TOKEN_WORD` with value `export`
        2. `TOKEN_WORD` with value `MY_VAR="Hello"`
2. **Parsing:**
    - Command:
        - **Args:** `["export", "MY_VAR=\"Hello\""]`
3. **Execution:**
    - **Identifies `export` as a built-in.**
    - **Processes the argument `MY_VAR="Hello"`:**
        - **Parses key as `MY_VAR` and value as `"Hello"`.**
    - **Updates the environment variable** `MY_VAR` with value `"Hello"`.

### **Example 5: Command with Variable Expansion**

**Input:** `echo $HOME`

**Process:**

1. **Lexing:**
    - Tokens:
        1. `TOKEN_WORD` with value `echo`
        2. `TOKEN_WORD` with value `$HOME`
2. **Parsing:**
    - Command:
        - **Args:** `["echo", "$HOME"]`
3. **Execution:**
    - **Expands `$HOME`** to the user's home directory (e.g., `/home/user`).
    - **Executes `echo /home/user`.**
    - **Output:** `/home/user`