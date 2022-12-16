all: clean rebuild
rebuild:
	gcc -Wall -Wextra -Werror -g s21_grep.c functions.c -o s21_grep `pcre-config --libs`
clean:
	rm -rf *.out
