CC=gcc
STDFLAGS= -I inc -std=c99 -Wall -Werror -Wextra
TSTFLAGS=-g $(STDFLAGS)
GCOV_FLAGS?=-fprofile-arcs -ftest-coverage
PREF_TEST_SRC= unit_test/
TEST_SRC=$(wildcard $(PREF_TEST_SRC)*.c)
LINUX_FLAGS=-lrt -lsubunit -lpthread -lm
LIBS=-lcheck -lncurses
SHELL=/bin/sh
SRC=$(wildcard ../../brick_game/tetris/*.c ../../gui/cli/*.c)
OBJ := game_sourse/*.o
OS:=$(SHELL uname -s)
TARGET=s21_tetris.a
PRE_TARGET=s21_tetris

ifeq ($(OS), (Linux))
OPEN_CMD=xdg-open
else
OPEN_CMD=open
endif

.DELETE_ON_ERROR :
		all : clean s21_tetris.a test

$(TARGET) : ${SRC}
		$(CC) -c $(STDFLAGS) $(SRC)
		ar rc $@ $(OBJ)
		ranlib $@
		cp $@ lib$@
		make clean_obj 

test : $(TARGET)
		$(CC) $(STDFLAGS) $(TEST_SRC) $< -o utest $(LIBS) $(LINUX_FLAGS)
		./utest

temp: 
		clear
		gcc tempv2.c -o test -lm
		./test

fsanitize : $(TARGET)
		$(CC) $(STDFLAGS) $(TEST_SRC) $< -o utest -g -fsanitize=address $(LIBS) $(LINUX_FLAGS)
		./utest

gcov_report : clean_test
		$(CC) $(STDFLAGS) $(GCOV_FLAGS) ${TEST_SRC} ${SRC} -o test $(LIBS) $(LINUX_FLAGS)
		./test
		lcov -t "stest" -o s21_test.info -c -d .
		genhtml -o report s21_test.info
		$(OPEN_CMD) ./report/index.html

clean_obj : 
		rm -rf game_sourse/*.o ./game_sourse/*.out
clean_lib :
		rm -rf game_sourse/*.a
clean_test :
		rm -rf game_sourse/*.gcda
		rm -rf game_sourse/*.gcno
		rm -rf game_sourse/*.info
		rm -rf game_sourse/test
		rm -rf game_sourse/report
		rm -rf game_sourse/*.gch
		rm -rf game_sourse/utest

clean : clean_test clean_lib clean_obj
		rm -rf game_sourse/RESULT_VALGRIND.txt
clang :
		cp ../materials/linters/.clang-format ../src/game/.clang-format; \
		clang-format -style=Google -n *.[c,h] ../brick_game/tetris/*.[c,h];
		clang-format -style=Google -n *.[c,h] ../gui/cli/*.[c,h];
		rm .clang-format

valgrind :
		valgrind game_sourse/--log-file=RESULT_VALGRIND.txt --leak-check=full --show-leak-kinds=all --show-reachable=yes --track-origins=yes ./utest -s
install:
		sudo apt-get install libncurses5-dev libncursesw5-dev
uninstall:
		sudo apt-get remove libncurses5-dev libncursesw5-dev
.PHONY : all
		all : $(TARGET)