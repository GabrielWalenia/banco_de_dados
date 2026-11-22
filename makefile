# Makefile para a geração do executavel fdtool

CC = gcc
FLAGS = -std=c11 -Wall -Wextra

all: main.c fdtool.c entrada.c lista.c
	$(CC) $(FLAGS) main.c fdtool.c entrada.c lista.c -o fdtool

debug: CFLAGS += -DDEBUG -g
debug: all

clean:
	rm fdtool