NAME = pipex

CC = cc

CFLAGS = -Werror -Wall -Wextra 
RM = rm -rf

SRCS = 	src/pipex.c\
		src/pipex_utils.c src/pipex_utils_2.c\
		Libft/libft.a\

$(NAME) :
	make all -C Libft
	cc $(CFLAGS) $(SRCS) -o $(NAME)


all : $(NAME)

fclean : clean
	$(RM) $(NAME)
	make fclean -C Libft

clean :
	$(RM) $(NAME)
	make clean -C Libft

re : fclean all
