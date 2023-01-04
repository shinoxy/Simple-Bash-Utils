CC = gcc -Wall -Werror -Wextra -std=c11 -g

all: s21_grep

s21_grep: s21_grep.o
	$(CC) -o s21_grep s21_grep.o `pcre-config --libs --libs-posix`

s21_grep.o: s21_grep.c
	$(CC) -c s21_grep.c `pcre-config --libs --libs-posix`

check:
	clang-format -style=Google -n *.c *.h

clean:
	rm -rf *.o *.out s21_grep

rebuild:
	$(MAKE) clean 
	$(MAKE) all

test1: rebuild
	sh test_1_grep.sh

test2: rebuild
	sh test_2_grep.sh

test3: rebuild
	sh test_3_grep.sh
