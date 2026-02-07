# ================================= Files ======================================

NAME	= Life
SOURCE	= $(SOURCE_DIR)
OBJS	= $(addprefix $(OBJS_DIR), $(SOURCE_LST:.c=.o))
SDL		= -L ./SDL/build/sdl
# ============================ Folder Structures ===============================

HEADERS		= ./includes/
SOURCE_DIR	= $(addprefix ./src/, main.c, graphics.c, life.c)
OBJS_DIR	= ./objs/
SOURCE_LST	= main.c $(ENGINE) $(MEMORY) $(UTILS) $(PARSER) $(MAP)
SDL_DIR		= ./SDL/build/

# ============================ Commands & Flags ===============================

CC			= cc
RM			= rm -rf
#AR			= ar -rcs
FLAGS		= -Wall -Wextra -Werror -g #-O3 -pg
LEAKS		= -g -fsanitize=address
DEBUG		= -DDEBUG
MAKE_FLAG	= --no-print-directory
SDL_FLAGS	= 

# =========================== Ansi Escape Codes ================================

ULINE	= \e[4m
BLINK	= \e[5m
BLACK 	= \e[1;30m
RED 	= \e[1;31m
GREEN 	= \e[1;32m
YELLOW 	= \e[1;33m
BLUE	= \e[1;34m
PURPLE 	= \e[1;35m
CYAN 	= \e[1;36m
WHITE 	= \e[1;37m
RESET	= \e[0m

# ================================ Rules =======================================
#For debug compilation make debug=1

all: $(NAME)

$(NAME): $(OBJS)
	echo "[$(CYAN)$(BLINK)Linking...$(RESET)]"
ifdef debug
	$(CC) $(FLAGS) $(LEAKS) $(SDL) -o $@ $^ #$(SDL_FLAGS)
else
	$(CC) $(FLAGS) $(SDL) -o $@ $^ #$(SDL_FLAGS)
endif
	echo "\n*************************$(GREEN)$(BLINK)    "\
		"[Compilation Sucessfull!]    $(RESET)*************************\n"

$(OBJS):
	echo "[$(PURPLE)$(BLINK)Compiling...$(RESET)] $(YELLOW)sources$(RESET)"
	mkdir -p objs
ifdef debug
	$(CC) $(FLAGS) $(LEAKS) -c $(SOURCE_DIR) -I $(HEADERS) $(DEBUG)
else
	$(CC) $(FLAGS) -c $(SOURCE_DIR) -I $(HEADERS)
endif
	mv $(SOURCE_LST:.c=.o) $(OBJS_DIR)

clean:
	make clean $(MAKE_FLAG) -C $(LIBFT_DIR)
	make clean $(MAKE_FLAG) -sC $(SDL_DIR)
	$(RM) $(OBJS)
	$(RM) $(OBJS_DIR)
	echo "\n\n++++++++++++++    $(ULINE)$(GREEN)Life's Objects have been" \
		"removed sucessfully$(RESET)    +++++++++++++++\n\n"

fclean: clean
	make fclean $(MAKE_FLAG) -C $(LIBFT_DIR)
	make clean $(MAKE_FLAG) -sC $(SDL_DIR)
	$(RM) $(NAME)
	$(RM) $(BONUS)
	echo "\n\n++++++++++++++    $(ULINE)$(GREEN)Life's libraries and "\
		"programs removed successfully$(RESET)    +++++++++++++++\n\n"

re: fclean all

leak: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(name) \
		maps/valid/test.cub
bleak: bonus 
	valgrind --leak-check=full --show-leak-kinds=all ./$(BONUS) \
		maps/valid/test.cub

run: re
	./$(NAME) maps/valid/test.cub

.SILENT:

.PHONY: all clean fclean re
