
NAME = pipex

SOURCES =	pipex.c pipex_utils.c \

OBJECTS = $(SOURCES:.c=.o)

GCC = gcc

CFLAGS = -Wall -Werror -Wextra -Iincludes

LIBFT = cd .. && cd libft && make

LIB = ../libft/libft.a

# ------------------------------ Rules ------------------------------
all: $(NAME)

$(NAME): comp_start pipex

comp_start:
	@$(LIBFT)

pipex: $(OBJECTS)
	$(GCC) $(CFLAGS) $(OBJECTS) $(LIB) -o $(NAME)

%.o: %.c
	$(GCC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJECTS)
	@cd .. && cd libft && make clean

fclean: clean
	@rm -rf $(NAME)
	@cd .. && cd libft && make fclean

re: fclean all

.PHONY: all pipex clean fclean re libft