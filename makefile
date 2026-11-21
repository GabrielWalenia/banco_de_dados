all:
	gcc -Wall -Wextra main.c lista.c entrada.c fdtool.c -o fdtool
debug:
	gcc -Wall -Wextra -g main.c lista.c fdtool.c entrada.c -o fdtool