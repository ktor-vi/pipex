
NAME = pipex


SRC = 	src/pipex.c            \
				src/parsing.c          \

INC = includes/pipex.h

CC = cc

FLAGS =  -Wall -Wextra -Werror

OBJ = $(SRC:.c=.o)

LIBFT_DIR = libft

LIBS = -L $(LIBFT_DIR) -lft

all: $(NAME)

$(NAME): $(OBJ)
	@make -C $(LIBFT_DIR)
	@cp $(LIBFT_DIR)/libft.a .
	$(CC) $(CFLAGS) $(OBJ) libft.a -fsanitize=address -o $@ 
	@rm libft.a

%.o: %.c $(INC)
	$(CC) $(FLAGS) -I $(INC) -o $@ -c $<

bonus: $(OBJS) $(BONUS_OBJS)
	ar rcs $(NAME) $(OBJS) $(BONUS_OBJS)

clean:
	@make fclean -C $(LIBFT_DIR)
	rm -f $(OBJ) $(BONUS_OBJS)

fclean: clean
	@make fclean -C $(LIBFT_DIR)
	rm -f $(NAME)

re: fclean all
