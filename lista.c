#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "fdtool.h"
struct fila *cria_fila()
{
    struct fila *f = (struct fila *)malloc(sizeof(struct fila));
    if (!f)
    {
        fprintf(stderr, "Falha ao alocar memória para a fila");
        return NULL;
    }
    f->front = NULL;
    f->back = NULL;

    return f;
}

void destroi_fila(struct fila *f)
{
    if (!f)
    {
        fprintf(stderr, "Ponteiro nulo!\n");
        return;
    }

    for (struct node *aux = f->front; aux != NULL; aux = aux->next)
    {
        free(f->front);
        f->front = aux->next;
    }
    free(f);
}

int insere_fila(char *inserir, struct fila *f)
{
    if (!inserir || !f)
    {
        fprintf(stderr, "inserir ou f nulo\n");
        return 1;
    }

    struct node *no = (struct node *)malloc(sizeof(struct node));

    if (!no)
    {
        fprintf(stderr, "Falha ao alocar memoria para o nó\n");
        return 1;
    }

    no->next = NULL;
    no->atr = malloc(sizeof(atributos));

    if (no->atr == NULL)
    {
        free(no);
        fprintf(stderr, "Falha ao alocar memoria para atributos\n");
        return 1;
    }

    memcpy(no->atr, inserir, sizeof(atributos));

    if (f->front == NULL)
    {

        f->front = no;
        f->back = no;
    }
    else
    {
        f->back->next = no;
        f->back = no;
    }

    return 0;
}

struct node *retira_fila(struct fila *f)
{

    if (f == NULL)
    {
        fprintf(stderr, "f nulo!\n");
        return NULL;
    }

    if (f->back == NULL && f->front == NULL)
    {
        fprintf(stderr, "fila já está vazia\n");
        return NULL;
    }
    struct node *aux = f->front;
    f->front = aux->next;
    if (f->back == f->front)
    {
        f->front = NULL;
        f->back = NULL;
        return aux;
    }
    else
    {
        return aux;
    }
}

int fila_vazia(struct fila *f)
{
    if (f->front == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}