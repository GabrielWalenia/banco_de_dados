#include <stdio.h>
#include <stdlib.h>
#include "fdtool.h"

static char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

struct atributo **closure(struct atributo *atr, struct dependencia *d, int tam)
{
  if (atr == NULL || d == NULL || tam < 0)
    return NULL;

  // Reserva o espaço maximo que esse fecho pode ter
  struct atributo **fecho = (struct atributo **)malloc(sizeof(struct atributo *) * tam);

  // Primeiro elemento que o fecho recebe é o proprio atributo

  for (int i = 0; i < d->num; i++)
  {
    if (d->lhs[i].attributes[0] == atr->attributes[0])
    {
      for (struct atributo *aux = &d->lhs[i]; aux != NULL; aux = aux->determina)
      {
        fecho[i] = aux;
        i++;
      }
    }
  }

  return fecho;
}

int mincover(struct dependencia *d)
{
  // cada dependência funcional possui apenas um atributo no lado direito;
  // não existem atributos desnecessários no lado esquerdo;
  // não existem dependências funcionais redundantes.

  if (d == NULL)
    return -1;

  int tam_anterior;
  struct atributo novo_atributo;
  for (int i = 0; i < d->num; i++)
  {
    // se o valor for composto então
    //   cria novas DF (numero de elementos do lado direito) com o mesmo lado esquerdo
    //   cada DF recebe um atributo que foi dividido
    // fimse
    if (d->rhs[i].tam > 1)
    {
      tam_anterior = d->num;
      d->num += d->rhs[i].tam;
      d->rhs = realloc(d->rhs, sizeof(struct atributo) * d->num);
      d->lhs = realloc(d->lhs, sizeof(struct atributo) * d->num);

      for (int j = tam_anterior; j < d->num; j++)
      {
        d->lhs[j] = d->lhs[i];
        // Possivel erro
        // for (int w = 0; w < d->rhs[i].tam; w++)
        //   novo_atributo.attributes[0] = d->rhs[i].attributes[w];
        // novo_atributo.determina = NULL;
        // novo_atributo.tam = 1;
      }
    }
  }

  for (int i = 0; i < d->num; i++)
  {
    // Se lado esquerdo == composto então
    //   Para j = 0 até num_atributos_unidos faca
    //     fecho <- fecho do atributo[j]
    //     se atributo do lado direito pertencer estiver contido em fecho de atributo[j]
    //       excluir atributos não relevantes
    //       parar
    //     fimse
    //   fimpara
    // fimse
  }

  for (int i = 0; i < d->num; i++)
  {
    // se dois elementos do lado esquerdo forem iguais
    //   verificar se tem alguem do outro apontando para ele
    //   se verdadeiro
    //     tirar dependencia
    //   fimse
    // fimse
  }
  for (int i = 0; i < d->num; i++)
  {
    // se dois elementos do lado esquerdo forem iguais e dois elementos do lado direito forem iguais
    //   retirar DF's até sobrar apenas uma
    // fimse
  }

  return 0;
}
struct atributo *keys(struct atributo *atr, struct dependencia *d, struct relacao *r)
{

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
  return 0;
}
int normalform(struct relacao *r)
{
  int forma = 0;
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
  return forma;
}
