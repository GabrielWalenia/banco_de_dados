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

    struct node *aux = f->front;
    while (aux)
    {
        struct node *prox = aux->next;
        free(aux->atr);
        free(aux);
        aux = prox;
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
    if (!f || f->front == NULL)
    {
        fprintf(stderr, "Erro ao retirar da fila\n");
        return NULL;
    }

    struct node *aux = f->front;
    f->front = aux->next;
    if (f->front == NULL)
    {
        f->back = NULL;
    }

    aux->next = NULL;
    return aux;
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