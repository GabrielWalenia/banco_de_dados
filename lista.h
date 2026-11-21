#ifndef __LISTA__
#define __LISTA__

struct node
{
    char *atr;
    struct node *next;
};

struct fila
{
    struct node *front;
    struct node *back;
};

struct fila *cria_fila();
void destroi_fila(struct fila *l);
int insere_fila(char *inserir, struct fila *l);
struct node *retira_fila(struct fila *f);
int fila_vazia(struct fila *f);
#endif