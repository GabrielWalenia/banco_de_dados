#ifndef __ENTRADA__
#define __ENTRADA__

#define TAM_ALFABETO 26

typedef char atributos[TAM_ALFABETO];
struct lista_dependencias;

FILE *abrir_arquivo(char *path);
struct lista_dependencias *gera_lista_dependencias(FILE *arq);
void gerar_atributos(char *str_attr, atributos set);
char *gerar_strings(atributos set);
void imprimir_fds(struct lista_dependencias *list);

// Funções para depuração
void print_debug(struct lista_dependencias *list);
void print_attr_debug(atributos attr);
#endif