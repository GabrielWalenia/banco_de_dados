#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "fdtool.h"
#include "entrada.h"
#include "lista.h"

#define VIOLATIONS_LIST_SIZE 64

atributos universo = {0};

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
        free(fecho);
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
  if (cobertura_minima->dep == NULL)
  {
    fprintf(stderr, "Erro ao alocar memória para a lista de dependencias\n");
    free(cobertura_minima);
    return NULL;
  }
  cobertura_minima->size = 0;

  separar_lado_direito(d, cobertura_minima);

  tirar_atributos_desnecessarios(cobertura_minima);

  tirar_transitividade(cobertura_minima);

  return cobertura_minima;
}

// ----------------------------------------------------------------------------------------
// Funcões da chave

bool eh_subconjunto(char *a, char *b)
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

bool esta_no_fecho(char *atr, char *fecho)
{
  for (int i = 0; i < TAM_ALFABETO; i++)
  {
    if (atr[i] && !fecho[i])
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

  struct fila *f;
  f = cria_fila();
  if (!f)
  {
    fprintf(stderr, "Falha ao alocar memória para fila");
    return NULL;
  }

  insere_fila(essenciais, f);

  int key_count = 0;
  int keys_size = 26;
  // o pior numero de chaves possivel que pode ter é 26 * 26
  atributos *keys = malloc(sizeof(atributos) * keys_size);
  if (keys == NULL)
  {
    fprintf(stderr, "ERRO ao alocar memória para as chaves\n");
    return NULL;
  }

  while (!fila_vazia(f))
  {
    struct node *atual;
    atributos atr = {0};

    atual = retira_fila(f);
    if (atual == NULL)
    {
      fprintf(stderr, "Erro ao retirar elemento da fila\n");
      break;
    }

    memcpy(atr, atual->atr, sizeof(atributos));
    free(atual->atr);
    free(atual);
    char *fecho = closure(atr, l);

    bool superchave = true;
    if (!esta_no_fecho(universo, fecho))
    {
      superchave = false;
    }

    free(fecho);
    if (superchave)
    {
      bool conjunto_minimo = true;
      for (int i = 0; i < key_count; i++)
      {
        if (eh_subconjunto(keys[i], atr))
        {
          conjunto_minimo = false;
        }
      }
      if (conjunto_minimo)
      {
        memcpy(keys[key_count], atr, sizeof(atributos));
        key_count++;
        if (key_count >= keys_size)
        {
          keys_size *= 2;
          atributos *tmp = realloc(keys, sizeof(atributos) * keys_size);
          if (!tmp)
          {
            fprintf(stderr, "Erro ao realocar memória");
            free(keys);
            destroi_fila(f);
            return NULL;
          }
          keys = tmp;
        }
      }
      continue;
    }

    for (int i = 0; i < TAM_ALFABETO; i++)
    {
      if (!universo[i] || atr[i])
      {
        continue;
      }

      atributos next = {0};
      memcpy(next, atr, sizeof(atributos));
      next[i] = 1;

      bool ja_eh_superchave = false;
      for (int j = 0; j < key_count; j++)
      {
        if (eh_subconjunto(keys[j], next))
        {
          ja_eh_superchave = true;
        }
      }
      if (!ja_eh_superchave)
      {
        insere_fila(next, f);
      }
    }
  }

  destroi_fila(f);
  *num_chaves = key_count;
  return keys;
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
  free(k);

  atributos violacoes_BCNF[VIOLATIONS_LIST_SIZE];
  int BCNF_RHS[VIOLATIONS_LIST_SIZE];
  int BCNF_count = 0;

  atributos violacoes_3NF[VIOLATIONS_LIST_SIZE];
  int _3FN_RHS[VIOLATIONS_LIST_SIZE];
  int _3FN_count = 0;

  for (int i = 0; i < cobertura_minima->size; i++)
  {
    struct dependencia dep = cobertura_minima->dep[i];
    int rhsIdx;

    for (int j = 0; j < TAM_ALFABETO; j++)
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

    atributos atr = {0};
    memcpy(atr, dep.lhs, sizeof(atributos));
    char *fecho = closure(atr, cobertura_minima);

    bool superchave = equals(fecho, universo);
    bool rhs_primo = atributo_primo[rhsIdx];
    free(fecho);

    if (!superchave)
    {
      memcpy(violacoes_BCNF[BCNF_count], dep.lhs, sizeof(atributos));
      BCNF_RHS[BCNF_count] = rhsIdx;
      BCNF_count++;
    }

    if (!superchave && !rhs_primo)
    {
      memcpy(violacoes_3NF[_3FN_count], dep.lhs, sizeof(atributos));
      _3FN_RHS[_3FN_count] = rhsIdx;
      _3FN_count++;
    }
  }

  if (BCNF_count > 0)
  {
    printf("BCNF: VIOLATIONS\n");
  }

  if (_3FN_count > 0)
  {
    printf("3NF: VIOLATIONS\n");
  }

  for (int i = 0; i < BCNF_count; i++)
  {
    char *str = gerar_strings(violacoes_BCNF[i]);
    printf("VIOLATION BCNF: %s->%c (%s not superkey)\n",
           str, 'A' + BCNF_RHS[i],
           str);
    free(str);
  }

  for (int i = 0; i < _3FN_count; i++)
  {
    char *str = gerar_strings(violacoes_BCNF[i]);
    printf("VIOLATION 3NF: %s->%c (%c not prime, %s not superkey)\n",
           str, 'A' + _3FN_RHS[i],
           'A' + _3FN_RHS[i], str);
  }
  liberar_lista(cobertura_minima);
}

//------------------------------------------------------------------------------------
// Funções de impressão

void imprimir_chaves(atributos *cKeys, int keys_count)
{
  for (int i = 0; i < keys_count; i++)
  {
    for (int j = 0; j < TAM_ALFABETO; j++)
    {
      if (cKeys[i][j] == true)
      {
        printf("%c", 'A' + j);
      }
    }
    printf("\n");
  }
}

void imprimir_dependencia(struct lista_dependencias *list)
{
  for (int i = 0; i < list->size; i++)
  {
    struct dependencia _fd = list->dep[i];
    for (int j = 0; j < TAM_ALFABETO; j++)
    {
      if (_fd.lhs[j] == true)
      {
        printf("%c", 'A' + j);
      }
    }
    printf("->");
    for (int j = 0; j < TAM_ALFABETO; j++)
    {
      if (_fd.rhs[j] == true)
      {
        printf("%c", 'A' + j);
      }
    }
    printf("\n");
  }
}

//---------------------------------------------------------------------------------------------
// Funções auxiliares
void liberar_lista(struct lista_dependencias *l)
{
  if (!l)
    return;
  free(l->dep);
  free(l);
}