## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Built-ins](#built-ins)
- [Externals](#externals)
- [Project_Structure](#project_structure)
- [Credits](#credits)

## usage of readline.supp in valgrind with flags:
valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all --suppressions=readline.supp ./minishell

## Overview
Minishell

Welcome to Minishell, a minimalistic shell implemented in C. This project is a simplified version of a Unix shell, designed to replicate some of the basic functionalities of popular shells like Bash. The primary goal of this project is to understand how a shell works internally, including command parsing, execution, environment management, and handling built-in commands.


## Features
- Command Parsing: Supports parsing of user input, including command names, arguments, quotes, and escape characters.
- Built-in Commands: Implements essential built-in commands such as echo, cd, pwd, export, unset, env, exit, and history.
- Pipes and Redirections: Supports piping between commands and input/output redirections (<, >, >>, <<).
- Environment Variable Handling: Allows setting, unsetting, and expanding environment variables.
- Command History: Keeps track of the commands entered during the session.
- Signal Handling: Handles interrupts like Ctrl+C and Ctrl+D gracefully.
- Execution of External Commands: Searches the system PATH to execute external commands.

## Installation
To build and run Minishell, you need:

* A Unix-like operating system (Linux, macOS).
* A C compiler that supports C99 or later (e.g., gcc).
* The readline library installed on your system.

Building Minishell

1. Clone the Repository

```bash
git clone https://github.com/yourusername/minishell.git
cd minishell
```
2. Compile the Source Code

Use the provided Makefile to build the project:

```bash
make
```
This will generate an executable named minishell.

3. Running Minishell
After building the project, you can start Minishell by running:

```bash
./minishell
```

## Usage
When you start Minishell, you will see a custom prompt:

```bash
📟 (s)hell >>
```
You can now start typing commands as you would in a regular shell.

## Built-ins
Minishell implements several built-in commands:
```bash
echo
```
Prints arguments to the standard output.

```bash
📟 (s)hell >> echo Hello, World!
Hello, World!
```
Changes the current directory.

```bash
cd
📟 (s)hell >> cd /path/to/directory
```
Prints the current working directory.

```bash
pwd
📟 (s)hell >> pwd
/Users/yourusername/minishell
```
Sets environment variables.

```bash
export
📟 (s)hell >> export MY_VAR="Hello"
```
Unsets environment variables.

```bash
unset
📟 (s)hell >> unset MY_VAR
```
Displays all environment variables.

```bash
env
📟 (s)hell >> env
```
Exits the shell.

```bash
exit
📟 (s)hell >> exit
```
Shows the command history.

```bash
history
📟 (s)hell >> history
1 echo Hello, World!
2 pwd
3 history
```
## Externals
Minishell can execute external commands by searching the system PATH.

```bash
📟 (s)hell >> ls -la
```
Pipes and Redirections
Pipes (|)
You can pipe the output of one command to another.

```bash
📟 (s)hell >> ls -la | grep ".c"
```
Output Redirection (>, >>)
Redirect the output of a command to a file.

```bash
📟 (s)hell >> echo "Hello" > file.txt
📟 (s)hell >> echo "World" >> file.txt
```
Input Redirection (<)
Use a file as input to a command.

```bash
📟 (s)hell >> sort < unsorted.txt
```
Heredoc (<<)
Read input until a delimiter is found.

```bash
📟 (s)hell >> cat << EOF
> This is a heredoc test.
> EOF
```
Environment Variables
You can use environment variables in your commands.

```bash
📟 (s)hell >> echo $HOME
/Users/yourusername
```
Command History
Use the history command to view your command history.

Signal Handling
Minishell handles signals to provide a better user experience.

Ctrl+C (SIGINT): Interrupts the current command and returns to the prompt.
Ctrl+D (EOF): Exits the shell.
Ctrl+\ (SIGQUIT): Ignored by the shell.

## Project_Structure
- minishell.c: Entry point of the application.
- include/: Contains header files.
- src/: Contains source files divided into logical units:
- Lexer: Tokenizes user input.
- Parser: Parses tokens into commands.
- Executor: Executes commands.
- Built-ins: Implements built-in commands.
- Environment: Manages environment variables.
- Signals: Handles signal interrupts.
- Utilities: Helper functions and utilities.
- Makefile: Build script for compiling the project.

## Credits

This project was developed in colaboration with https://github.com/maxszlichta

This project was developed for educational purposesfor 42 Cursus to gain a deeper understanding of how shells work internally. It is not intended to replace fully-featured shells like Bash or Zsh.

If you have any questions or need further assistance, feel free to reach out!

