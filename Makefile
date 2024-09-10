
NAME = minishell

SOURCES =	src/minishell.c src/utils.c \

OBJECTS = $(SOURCES:.c=.o)

GCC = gcc

CFLAGS = -Wall -Werror -Wextra -Iincludes

PIPEX = cd include && cd pipex && make

LIB = include/libft/libft.a

# ------------------------------ Rules ------------------------------
all: $(NAME)

$(NAME): comp_begin minishell

comp_begin:
	@$(PIPEX)

minishell: $(OBJECTS)
	$(GCC) $(CFLAGS) $(OBJECTS) $(LIB) -o $(NAME)

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
