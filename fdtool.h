#ifndef __FDTOOL__
#define __FDTOOL__

#define TAM_ALFABETO 26

typedef char atributos[TAM_ALFABETO];
extern atributos universo;
struct dependencia
{
    atributos lhs; // atributos que estão do lado esquerdo
    atributos rhs; // atributos que estão do lado direito
};

struct lista_dependencias
{
    struct dependencia *dep;
    int size;
};

// Calcula o fecho do atributo
char *closure(atributos atr, struct lista_dependencias *d);

// Calcula a cobertura minima da relação
struct lista_dependencias *mincover(struct lista_dependencias *d);

// Calcula as chaves candidatas
atributos *keys(struct lista_dependencias *l, int *num_chaves);

// Imprime chaves candidatas
void imprimir_chaves(atributos *cKeys, int keys_count);

// Verifica se está na forma normal 3FN e BCNF
void normalform(struct lista_dependencias *d);

void imprimir_dependencia(struct lista_dependencias *list);

void liberar_lista(struct lista_dependencias *l);

#endif
