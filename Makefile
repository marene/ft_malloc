ifeq ($(HOSTTYPE),)
	HOSTTYPE = $(shell uname -m)_$(shell uname -s)
endif
NAME =			libft_malloc_$(HOSTTYPE).so
UNIT_NAME =		tests
LINKNAME =		libft_malloc.so
SRCS_DIR =		./srcs/
INCLUDES_DIR =	./includes/
OBJ_DIR =		./obj/
LIBFT_DIR =		./libft/
UNIT_DIR =		./unit_tests/
LIBFT_NAME =	libft.a
SRCS =			defragment_memory.c\
				dump_alloc_mem.c\
				free.c\
				init.c\
				malloc.c\
				realloc.c\
				show_alloc_mem.c
OBJ =			$(SRCS:.c=.o)
UNIT_SRCS =		main.c\
				unit_malloc.c\
				unit_realloc.c
UNIT_OBJS =		$(UNIT_SRCS:.c=.o)
LIBFT =			$(LIBFT_DIR)libft.a
CC =			clang
CFLAGS =		-Wall -Wextra -Werror -g -I $(INCLUDES_DIR) -I $(LIBFT_DIR)includes -o
LDFLAGS =		-L $(LIBFT_DIR) -lft -shared -o

all: $(OBJ_DIR) $(LIBFT_NAME) $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(LIBFT_NAME):
	make -C $(LIBFT_DIR) all

$(NAME): $(addprefix $(OBJ_DIR), $(OBJ))
	@rm -f $(LINKNAME)
	$(CC) $(LDFLAGS) $@ $^
	@ln -s $(NAME) $(LINKNAME)

$(OBJ_DIR)%.o: $(SRCS_DIR)%.c
	$(CC) $(CFLAGS) $@ -c $<
	
unit: $(UNIT_NAME)

$(UNIT_NAME): 
	$(CC) $(CFLAGS) $@ -L $(LIBFT_DIR) -lft $(LINKNAME) $(addprefix $(UNIT_DIR), $(UNIT_SRCS))

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)
	rm -rf $(LINKNAME)

re: fclean all

norme:
	norminette $(addprefix $(SRCS_DIR), $(SRCS))

.PHONY: all clean fclean re unit $(UNIT_NAME)
