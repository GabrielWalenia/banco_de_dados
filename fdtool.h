#ifndef __FDTOOL__
#define __FDTOOL__

#define TAM_ALFABETO 26

// // A
// struct atributo{
//   char valor;
// };

// A->B
struct dependencia
{
    int num;              // Número de dependencias funcionais
    struct atributo *lhs; // Vetor de todos os atributos que estão do lado esquerdo
    struct atributo *rhs; // Vetor de todos os atributos que estão do lado direito
};

// struct que representa a relação R(A, B, ...)

// struct que representa a dependencia funcianal A->B
// struct dependencia
// {
//     struct atributo *lhs;
//     struct atributo *rhs;
// };

// struct que representa o conjuto de dependencias funcionais
// struct restricoes
// {
//     struct dependencia *dependencias;
// };
// O atributo possui o valor e um ponteiro para o seu determinado
// se houver
struct relacao
{
    struct atributo *r;
    int num_elementos;
};

struct lista_t
{
    struct atributo *primeiro;
    struct atributo *ultimo;
    int num;
};
struct atributo
{
    int tam;
    char attributes[TAM_ALFABETO];
    struct atributo *determina;
};

// Calcula o fecho do atributo
struct atributo **closure(struct atributo *atr, struct dependencia *d, int tam);
// Calcula a cobertura minima da relação
int mincover(struct dependencia *d);
// Calcula as chaves candidatas
struct atributo *keys(struct atributo *atr, struct dependencia *d, struct relacao *r);
// Verifica se está na forma normal 3FN e BCNF
int normalform(struct relacao *r);

#endif
