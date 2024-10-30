
NAME = minishell

SOURCES =	src/minishell.c \
			src/builtins/builtins.c src/builtins/export.c src/builtins/env.c src/builtins/cd.c \
			src/executor/executor.c src/executor/executor_builtins.c src/executor/executor_paths.c src/executor/executor_utils.c \
			src/lexer/lexer.c src/lexer/lexer_utils.c src/lexer/lexer_tokens.c \
			src/utils.c src/parser.c src/signals.c \
			src/expander.c src/pipes.c \
			src/redirections.c \
			
OBJECTS = $(SOURCES:.c=.o)

GCC = gcc

CFLAGS = -Wall -Werror -Wextra -Iincludes -fPIE

LIBFT = cd include && cd libft && make

LIB = include/libft/libft.a

READLINE = -L/usr/include -lreadline

# ------------------------------ Rules ------------------------------
all: $(NAME)

$(NAME): comp_begin minishell

comp_begin:
	@$(LIBFT)

minishell: $(OBJECTS)
	$(GCC) $(CFLAGS) $(OBJECTS) $(LIB) $(READLINE) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJECTS)
	@cd include && cd libft && make clean

fclean: clean
	@rm -rf $(NAME)
	@cd include && cd libft && make fclean

re: fclean all

.PHONY: all clean fclean re
