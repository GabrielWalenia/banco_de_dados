#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "fdtool.h"
#include "entrada.h"

#define VIOLATIONS_LIST_SIZE 64

// -----------------------------------------------------------------------------------------
// Funções do fecho

int verificar_lado_equerdo(atributos lhs, atributos fecho)
{
  for (int i = 0; i < TAM_ALFABETO; i++)
  {
    if (lhs[i] == 1 && fecho[i] == 0)
    {
      return -1;
    }
  }
  return 0;
}

char *closure(atributos atr, struct lista_dependencias *d)
{
  // fecho = os proprios atributos
  // Lembrar de fechar
  char *fecho = malloc(TAM_ALFABETO);
  if (!fecho)
  {
    return NULL;
  }
  for (int w = 0; w < TAM_ALFABETO; w++)
  {
    fecho[w] = atr[w];
  }

  bool incrementou = true;
  while (incrementou)
  {
    incrementou = false;
    for (int i = 0; i < d->size; i++)
    {
      struct dependencia momento = d->dep[i];

      if (verificar_lado_equerdo(momento.lhs, fecho) == 0)
      {
        for (int j = 0; j < TAM_ALFABETO; j++)
        {
          if (momento.rhs[j] && !fecho[j])
          {
            fecho[j] = 1;
            incrementou = true;
          }
        }
      }
    }
  }
  return fecho;
}

// -----------------------------------------------------------------------------------------
// Funções da cobertura minima

void separar_lado_direito(struct lista_dependencias *antiga,
                          struct lista_dependencias *c)
{
  struct dependencia d;

  for (int i = 0; i < antiga->size; i++)
  {
    d = antiga->dep[i];
    for (int j = 0; j < TAM_ALFABETO; j++)
    {
      if (d.rhs[j])
      {
        struct dependencia nova_dependencia = {0};

        memcpy(nova_dependencia.lhs, d.lhs, sizeof(d.lhs));
        nova_dependencia.rhs[j] = 1;

        c->dep[c->size] = nova_dependencia;
        c->size++;
      }
    }
  }
}

//  aux->size = c->size -1;
//     aux->dep = malloc(sizeof(struct dependencia) * aux->size);

//     int cont = 0;

//     for(int j = 0; j < c->size; j++){
//       if(i != j){
//         aux->dep[cont] = c->dep[j];
//         cont++;
//       }
//     }

bool verificar_conjuto(atributos rhs, char *fecho)
{
  for (int i = 0; i < TAM_ALFABETO; i++)
  {
    // Se o atributo de rhs está no fecho
    if (rhs[i] && fecho[i])
    {
      return true;
    }
  }
  return false;
}
void tirar_atributos_desnecessarios(struct lista_dependencias *c)
{
  for (int i = 0; i < c->size; i++)
  {
    struct dependencia *d = &c->dep[i];
    for (int j = 0; j < TAM_ALFABETO; j++)
    {
      if (d->lhs[j])
      {
        atributos temp = {0};
        memcpy(temp, d->lhs, sizeof(atributos));
        temp[j] = 0;

        atributos atr;
        char *fecho;
        memcpy(atr, temp, sizeof(atributos));
        fecho = closure(atr, c);

        bool desnecessario = false;
        if (verificar_conjuto(d->rhs, fecho))
        {
          desnecessario = true;
        }

        if (desnecessario)
          d->lhs[j] = 0;
      }
    }
  }
}
void tirar_transitividade(struct lista_dependencias *c)
{
  for (int i = 0; i < c->size; i++)
  {
    struct dependencia d = c->dep[i];
    struct lista_dependencias temp;
    temp.size = c->size - 1;

    temp.dep = malloc(sizeof(struct dependencia) * temp.size);

    int cont = 0;
    for (int j = 0; j < c->size; j++)
    {
      if (i != j)
      {
        temp.dep[cont] = c->dep[j];
        cont++;
      }
    }

    atributos atr = {0};
    memcpy(atr, d.lhs, sizeof(atributos));
    char *fecho = closure(atr, &temp);

    bool redundante = false;
    for (int j = 0; j < TAM_ALFABETO; j++)
    {
      if (verificar_conjuto(d.rhs, fecho))
      {
        redundante = true;
      }
    }

    if (redundante)
    {
      c->dep[i] = c->dep[c->size - 1];
      c->size--;
      i--;
    }

    free(temp.dep);
  }
}

struct lista_dependencias *mincover(struct lista_dependencias *d)
{
  // cada dependência funcional possui apenas um atributo no lado direito;
  // não existem atributos desnecessários no lado esquerdo;
  // não existem dependências funcionais redundantes.

  if (d == NULL)
  {
    fprintf(stderr, "Paramentro lista_dependencias é nulo\n");
    return NULL;
  }

  struct lista_dependencias *cobertura_minima = malloc(sizeof(struct lista_dependencias));
  if (cobertura_minima == NULL)
    return NULL;

  // Por enquanto a cobertura minima é uma cópia das dependencias passadas

  cobertura_minima->dep = malloc(sizeof(struct dependencia) * d->size * TAM_ALFABETO);
  cobertura_minima->size = 0;

  separar_lado_direito(d, cobertura_minima);

  tirar_atributos_desnecessarios(cobertura_minima);

  tirar_transitividade(cobertura_minima);

  return cobertura_minima;
}

// ----------------------------------------------------------------------------------------
// Funcões da chave

// MUDAR
atributos universo = {0};

typedef struct Node
{
  /**
   * @var Conjunto de atributos.
   */
  atributos attrs;

  /**
   * @var Ponteiro para próximo nó.
   */
  struct Node *next;
} Node;

/**
 * @brief Esta estrutura define uma fila. Usada para
 * encontrar as chaves candidatas usando BFS.
 */
typedef struct Queue
{
  /**
   * var Ponteiro para o início da fila;
   */
  struct Node *front;

  /**
   * var Ponteiro para o final da fila;
   */
  struct Node *back;
} Queue;

/**
 * @brief Inicializa uma fila.
 *
 * @param q Uma fila.
 */
static void queue_init(Queue *q);

/**
 * @brief Verifica se uma fila está vazia.
 *
 * @param q Uma fila.
 */
static bool queue_empty(Queue *q);

/**
 * @brief Enfileira um conjunto de atributos numa fila.
 *
 * @param q Uma fila.
 * @param attrSet Conjunto de atributos.
 */
static void enqueue(Queue *q, atributos attrSet);

/**
 * @brief Retira um conjunto de atributos de uma fila.
 *
 * @param q Uma fila.
 * @param out Conjunto de atributos (saída).
 */
static void dequeue(Queue *q, atributos out);

/**
 * @brief Verifica se conjunto a é um subconjunto do conjunto b.
 *
 * @param a Conjunto de atributos.
 * @param b Conjunto de atributos.
 * @return True, caso a seja subconjunto de b, False, caso contrário.
 */
static bool isSubset(atributos a, atributos b);

/**
 * @brief Verifica se o conjunto a é coberto pelo fecho.
 *
 * @param a Conjunto de atributos.
 * @param closure Conjunto de atributos.
 * @return True, caso a seja coberto pelo fecho, False, caso contrário.
 */
static bool isCoveredByClosure(atributos a, atributos closure);

static void queue_init(Queue *q)
{
  q->back = NULL;
  q->front = NULL;
}

static bool queue_empty(Queue *q)
{
  return q->front == NULL;
}

static void enqueue(Queue *q, atributos attrSet)
{
  Node *node = (Node *)malloc(sizeof(Node));
  if (node == NULL)
  {
    printf("ERROR! Couldn't allocate memory for Queue Node at file %s at line "
           "%d\n",
           __FILE__, __LINE__);
    exit(1);
  }
  memcpy(node->attrs, attrSet, sizeof(atributos));
  node->next = NULL;

  if (q->back == NULL)
  {
    q->front = q->back = node;
  }
  else
  {
    q->back->next = node;
    q->back = node;
  }
}

static void dequeue(Queue *q, atributos out)
{
  if (q->front == NULL)
  {
    return;
  }

  Node *temp = q->front;
  memcpy(out, temp->attrs, sizeof(atributos));
  q->front = temp->next;
  if (q->front == NULL)
  {
    q->back = NULL;
  }
  free(temp);
}

static bool isSubset(atributos a, atributos b)
{
  for (int i = 0; i < TAM_ALFABETO; i++)
  {
    if (a[i] && !b[i])
    {
      return false;
    }
  }
  return true;
}

static bool isCoveredByClosure(atributos a, atributos closure)
{
  for (int i = 0; i < TAM_ALFABETO; i++)
  {
    if (a[i] && !closure[i])
    {
      return false;
    }
  }
  return true;
}

atributos *keys(struct lista_dependencias *l, int *num_chaves)
{
  atributos essenciais = {0};
  atributos rhs = {0};

  for (int i = 0; i < l->size; i++)
  {
    for (int j = 0; j < TAM_ALFABETO; j++)
    {
      if (l->dep[i].rhs[j])
      {
        rhs[j] = 1;
      }
    }
  }
  for (int i = 0; i < TAM_ALFABETO; i++)
  {
    if (universo[i] && !rhs[i])
    {
      essenciais[i] = true;
    }
  }

  Queue q;
  queue_init(&q);
  enqueue(&q, essenciais);

  int key_count = 0;
  int keys_size = 32;
  atributos *keys = malloc(sizeof(atributos) * keys_size);
  if (keys == NULL)
  {
    printf("ERROR! Couldn't allocate memory to candidate keys array at file %s "
           "at line %d\n",
           __FILE__, __LINE__);
    exit(1);
  }

  while (!queue_empty(&q))
  {
    atributos current = {0};
    atributos closureSet = {0};

    dequeue(&q, current);
    memcpy(closureSet, current, sizeof(atributos));

    char *fecho = closure(closureSet, l);

    bool isSuperKey = true;
    if (!isCoveredByClosure(universo, fecho))
    {
      isSuperKey = false;
    }

    if (isSuperKey)
    {
      bool isKeyMinimum = true;
      for (int i = 0; i < key_count; i++)
      {
        if (isSubset(keys[i], current))
        {
          isKeyMinimum = false;
        }
      }

      if (isKeyMinimum)
      {
        memcpy(keys[key_count], current, sizeof(atributos));
        key_count++;
        if (key_count >= keys_size)
        {
          keys_size *= 2;
          keys = realloc(keys, sizeof(atributos) * keys_size);
        }
      }

      continue;
    }

    for (size_t i = 0; i < TAM_ALFABETO; i++)
    {
      if (!universo[i] || current[i])
      {
        continue;
      }

      atributos next = {0};
      memcpy(next, current, sizeof(atributos));
      next[i] = true;

      bool isAlreadySuperKey = false;
      for (int j = 0; j < key_count; j++)
      {
        if (isSubset(keys[j], next))
        {
          isAlreadySuperKey = true;
        }
      }

      if (!isAlreadySuperKey)
      {
        enqueue(&q, next);
      }
    }
  }

  *num_chaves = key_count;
  return keys;
  // Achar superchaves com o fecho
  // Para cada d->lhs[i] pertencente a d faca
  //   calcular fecho do atributo
  //   se fecho == R
  //     superchave <- atr
  //     chave_candidata <- atr
  //     para cada subconjunto de dhs[i] faca
  //     se fecho(subconjunto) == r
  //       chave_candidata <- subconjunto
  //     fimse
  //   fimse
  // fimpara

  // Calcular chaves candidatas
  // Retornar conjunto de chaves candidatas
}

// --------------------------------------------------------------------------------
// Funções da forma normal

static bool equals(char *a, char *b)
{
  for (int i = 0; i < TAM_ALFABETO; i++)
  {
    if (a[i] != b[i])
    {
      return false;
    }
  }
  return true;
}

void normalform(struct lista_dependencias *d)
{

  struct lista_dependencias *cobertura_minima = mincover(d);

  int num_chaves = 0;
  atributos *k = keys(cobertura_minima, &num_chaves);

  atributos atributo_primo = {0};

  for (int i = 0; i < num_chaves; i++)
  {
    for (int j = 0; j < TAM_ALFABETO; j++)
    {
      if (k[i][j])
      {
        atributo_primo[j] = true;
      }
    }
  }

  atributos BCNFViolationsList[VIOLATIONS_LIST_SIZE];
  int BCNF_RHS[VIOLATIONS_LIST_SIZE];
  int BCNFCount = 0;

  atributos _3FNViolationsList[VIOLATIONS_LIST_SIZE];
  int _3FN_RHS[VIOLATIONS_LIST_SIZE];
  int _3FNCount = 0;

  for (int i = 0; i < cobertura_minima->size; i++)
  {
    struct dependencia dep = cobertura_minima->dep[i];
    int rhsIdx;

    for (size_t j = 0; j < TAM_ALFABETO; j++)
    {
      if (dep.rhs[j])
      {
        rhsIdx = j;
      }
    }

    if (dep.lhs[rhsIdx])
    {
      continue;
    }

    atributos closureSet = {0};
    memcpy(closureSet, dep.lhs, sizeof(atributos));
    char *fecho = closure(closureSet, cobertura_minima);

    bool isSuperkey = equals(fecho, universo);
    bool rhsIsPrime = atributo_primo[rhsIdx];

    if (!isSuperkey)
    {
      memcpy(BCNFViolationsList[BCNFCount], dep.lhs, sizeof(atributos));
      BCNF_RHS[BCNFCount] = rhsIdx;
      BCNFCount++;
    }

    if (!isSuperkey && !rhsIsPrime)
    {
      memcpy(_3FNViolationsList[_3FNCount], dep.lhs, sizeof(atributos));
      _3FN_RHS[_3FNCount] = rhsIdx;
      _3FNCount++;
    }
  }

  if (BCNFCount > 0)
  {
    printf("BCNF: VIOLATIONS\n");
  }

  if (_3FNCount > 0)
  {
    printf("3NF: VIOLATIONS\n");
  }

  for (int i = 0; i < BCNFCount; i++)
  {
    printf("VIOLATION BCNF: %s->%c (%s not superkey)\n",
           gerar_strings(BCNFViolationsList[i]), 'A' + BCNF_RHS[i],
           gerar_strings(BCNFViolationsList[i]));
  }

  for (int i = 0; i < _3FNCount; i++)
  {
    printf("VIOLATION 3NF: %s->%c (%c not prime, %s not superkey)\n",
           gerar_strings(_3FNViolationsList[i]), 'A' + _3FN_RHS[i],
           'A' + _3FN_RHS[i], gerar_strings(_3FNViolationsList[i]));
  }
}
// int forma = 4;
// if (!determinante_superchave)
// {
//   forma = 3;
// }
// if (dependencia_funcional_transitiva)
// {
//   forma = 2;
// }
// if (dependencia_funcional_parcial)
// {
//   forma = 1;
// }

// Ver se não existem atributos compostos ou multivalorados (1FN)
// Ver se não existem dependencias funcionais parciais (2FN)
// Ver se não existem dependencias funcionais transitivas (3FN)
// Ver se todo lado determinante é uma superchave (BCNF)