CC = gcc -Wall -Werror -Wextra -std=c11 -g

all: s21_cat

s21_cat: s21_cat.o
	$(CC) -o s21_cat s21_cat.o

s21_cat.o: s21_cat.c
	$(CC) -c s21_cat.c

check:
	clang-format -style=Google -n *.c *.h

clean:
	rm -rf *.o *.out s21_cat

rebuild:
	$(MAKE) clean
	$(MAKE) s21_cat

test: rebuild
	sh test_1_cat.sh
	sh test_2_cat.sh