NAME		= pipex
LIBFT		= libft.a

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -g3
IFLAGS	= -Ilibft/include -Iinclude

SRC		= \
		  px_main.c		px_error.c	px_exec_pipex.c	\
		  px_get_path.c	px_split.c

BSRC	= \
		  px_main_bonus.c		px_error_bonus.c	px_exec_pipex_bonus.c		\
		  px_get_path_bonus.c	px_split_bonus.c	px_input_is_heredoc_bonus.c	\
		  px_init_input_fd_bonus.c

LIBFT_DIR	= libft/
SRC_DIR		= src/
BSRC_DIR	= src_bonus/

vpath %.c $(SRC_DIR) $(BSRC_DIR)

GREEN	= \e[32m
CYAN	= \e[36m
WHITE	= \e[0m

.PHONY: all fclean clean re bonus

all: $(NAME)

bonus: $(BSRC) $(LIBFT)
	@$(CC) $(CFLAGS) $(IFLAGS) $^ -o $(NAME)
	@echo "Compiling $(GREEN)bonus$(WHITE)..."

$(NAME): $(SRC) $(LIBFT)
	@$(CC) $(CFLAGS) $(IFLAGS) $^ -o $@
	@echo "Compiling $(GREEN)$(NAME)$(WHITE)..."

$(LIBFT):
	@echo "Making $(GREEN)$(LIBFT)$(WHITE)..."
	@make -C $(LIBFT_DIR)
	@cp $(addprefix $(LIBFT_DIR), $(LIBFT)) $(LIBFT)

fclean: clean
	@rm -f $(NAME)
	@echo "Removing program files..."

clean:
	@make -C $(LIBFT_DIR) clean
	@rm -f $(LIBFT) $(LIBFT_DIR)/$(LIBFT)
	@echo "Removing $(GREEN)$(LIBFT)$(WHITE)..."

re: fclean all
