all:
	gcc -Wall -Wextra main.c fdtool.c entrada.c -o fdtool
debug:
	gcc -Wall -Wextra -g main.c fdtool.c entrada.c -o fdtool