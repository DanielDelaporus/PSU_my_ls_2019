##
## EPITECH PROJECT, 2019
## Makefile
## File description:
## Makefile to build project
##

CC = gcc
FLAGS = -Wall -Wextra
SRC	= apply_flags.c \
			flag_l.c \
			info_file.c \
			my_ls.c
OBJ = $(SRC:.c=.o) $(MAIN:.c=.o)
NAME = my_ls
LIB_FOLDER = ./lib/my
LIB_NAME = my
TEST =	tests/test.c
TEST_FLAG =	--coverage -lcriterion
TEST_NAME =	unit_tests

all:		$(NAME)

$(NAME):	$(OBJ)
		make -C $(LIB_FOLDER)
		$(CC) $(FLAGS) -o $(NAME) $(SRC) -L$(LIB_FOLDER) -l$(LIB_NAME) -g3

clean:
		rm -f $(OBJ)
		rm -f $(OBJ_TEST)
		cd $(LIB_FOLDER) && make clean
		rm -f *~
		rm -f *#

fclean:		clean
		cd $(LIB_FOLDER) && make fclean
		rm -f $(NAME)
		rm -f $(TEST_NAME)
		rm -f *.gcda
		rm -f *.gcno

tests_run:	$(OBJ_TEST)
		make -C $(LIB_FOLDER)
		$(CC) $(FLAGS) -o $(TEST_NAME) $(TEST) $(SRC) $(TEST_FLAG) -L$(LIB_FOLDER) -l$(LIB_NAME) && ./$(TEST_NAME)

re:		fclean all
		cd $(LIB_FOLDER) && make re

auteur:
		echo $(USER) > auteur

.PHONY: 	all clean fclean re tests_run
