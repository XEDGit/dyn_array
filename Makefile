NAME := libdynarr.a

FILES := dyn_array.c

SRC_DIR := src

SRC := $(addprefix $(SRC_DIR)/, $(FILES))

OBJ_DIR := obj

OBJ := $(addprefix $(OBJ_DIR)/, $(FILES:.c=.o))

FLAGS := -c -I includes

all: $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(FLAGS) $< -o $@

$(OBJ_DIR):
	mkdir -p obj

$(NAME): $(OBJ)
	ar -cq $@ $^
	$(CC) main.c -I includes $@ -o dynarr

re: fclean all

d: FLAGS = -g -fsanitize=address
d: all

s: FLAGS = -g
s: all

red: FLAGS = -g -fsanitize=address
red: re

res: FLAGS = -g
res: re

fclean:
	rm -rf $(NAME) $(OBJ_DIR)