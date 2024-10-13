
NAME = minishell

SOURCES =	src/minishell.c src/utils.c src/lexer.c src/parser.c src/executor.c src/signals.c \
			src/builtins/builtins.c src/builtins/export.c src/expander.c \

OBJECTS = $(SOURCES:.c=.o)

GCC = gcc

CFLAGS = -Wall -Werror -Wextra -Iincludes -fPIE

PIPEX = cd include && cd pipex && make

LIB = include/libft/libft.a

READLINE = -L/usr/include -lreadline

# ------------------------------ Rules ------------------------------
all: $(NAME)

$(NAME): comp_begin minishell

comp_begin:
	@$(PIPEX)

minishell: $(OBJECTS)
	$(GCC) $(CFLAGS) $(OBJECTS) $(LIB) $(READLINE) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJECTS)
	@cd include && cd pipex && make clean

fclean: clean
	@rm -rf $(NAME)
	@cd include && cd pipex && make fclean

re: fclean all

.PHONY: all clean fclean re
