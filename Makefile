NAME = push_swap
BONUS_NAME = checker
CC = cc
CFLAGS = -Wall -Wextra -Werror
INCLUDES = -I includes -I libft

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SRCS = src/main.c \
	   src/parse_args.c \
	   src/parse_utils.c \
	   src/stack_init.c \
	   src/stack_ops_swap.c \
	   src/stack_ops_push.c \
	   src/stack_ops_rotate.c \
	   src/stack_ops_revrot.c \
	   src/ops_out_swap.c \
	   src/ops_out_rot.c \
	   src/ops_out_revrot.c \
	   src/normalize.c \
	   src/disorder.c \
	   src/sort_tiny.c \
	   src/sort_simple.c \
	   src/sort_medium.c \
	   src/sort_medium_utils.c \
	   src/sort_complex.c \
	   src/sort_adaptive.c \
	   src/bench.c \
	   src/utils.c

BONUS_SRCS = bonus/checker_main_bonus.c \
			 bonus/checker_exec_bonus.c \
			 src/parse_args.c \
			 src/parse_utils.c \
			 src/stack_init.c \
			 src/stack_ops_swap.c \
			 src/stack_ops_push.c \
			 src/stack_ops_rotate.c \
			 src/stack_ops_revrot.c \
			 src/ops_out_swap.c \
			 src/ops_out_rot.c \
			 src/ops_out_revrot.c \
			 src/normalize.c \
			 src/utils.c

OBJS = $(SRCS:.c=.o)
BONUS_OBJS = $(BONUS_SRCS:.c=.o)

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

bonus: $(LIBFT) $(BONUS_OBJS)
	$(CC) $(CFLAGS) $(BONUS_OBJS) $(LIBFT) -o $(BONUS_NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJS) $(BONUS_OBJS)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME) $(BONUS_NAME)

re: fclean all

.PHONY: all clean fclean re bonus
