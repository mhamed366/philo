NAME = philo
SRC =	SRCS/main.c\
		SRCS/utils.c

libft_bin = libft/libft.a
OBJ = $(SRC:.c=.o)
CC = cc
CFLAGS = -Wall -Wextra -Werror -g

all : makelibft $(NAME)

makelibft : 
	@make -C libft/

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) -lpthread -o $(NAME) $(OBJ) $(libft_bin)

cleanLibft :
	@make -C libft/ clean

clean : cleanLibft
	@rm -rf $(OBJ)
	@echo "Done"

re : fclean all

fclean : clean
	@rm -rf libft/libft.a
	@rm -rf $(NAME)