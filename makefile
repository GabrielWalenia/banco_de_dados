all:
	gcc -Wall -Wextra main.c fdtool.c -o fdtool
debug:
	gcc -Wall -Wextra -g main.c fdtool.c -o fdtool