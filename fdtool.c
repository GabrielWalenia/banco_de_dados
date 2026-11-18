#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fdtool.h"

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

void separar_lado_direito(struct lista_dependencias *c)
{
}
void tirar_atributos_desnecessarios(struct lista_dependencias *c)
{
}
void tirar_transitividade(struct lista_dependencias *c)
{
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

  struct lista_dependencias *cobertura_minima;
  // Por enquanto a cobertura minima é uma cópia das dependencias passadas
  cobertura_minima->size = d->size;
  cobertura_minima->dep = malloc(sizeof(struct dependencia) * cobertura_minima->size);

  separar_lado_direito(cobertura_minima);
  tirar_atributos_desnecessarios(cobertura_minima);
  tirar_transitividade(cobertura_minima);

  // for (int i = 0; i < d->num; i++)
  // {
  //   // se o valor for composto então
  //   //   cria novas DF (numero de elementos do lado direito) com o mesmo lado esquerdo
  //   //   cada DF recebe um atributo que foi dividido
  //   // fimse
  //   if (d->rhs[i].tam > 1)
  //   {
  //     tam_anterior = d->num;
  //     d->num += d->rhs[i].tam;
  //     d->rhs = realloc(d->rhs, sizeof(struct atributo) * d->num);
  //     d->lhs = realloc(d->lhs, sizeof(struct atributo) * d->num);

  //     for (int j = tam_anterior; j < d->num; j++)
  //     {
  //       d->lhs[j] = d->lhs[i];
  //       // Possivel erro
  //       // for (int w = 0; w < d->rhs[i].tam; w++)
  //       //   novo_atributo.attributes[0] = d->rhs[i].attributes[w];
  //       // novo_atributo.determina = NULL;
  //       // novo_atributo.tam = 1;
  //     }
  //   }
  // }

  // for (int i = 0; i < d->num; i++)
  // {
  //   // Se lado esquerdo == composto então
  //   //   Para j = 0 até num_atributos_unidos faca
  //   //     fecho <- fecho do atributo[j]
  //   //     se atributo do lado direito pertencer estiver contido em fecho de atributo[j]
  //   //       excluir atributos não relevantes
  //   //       parar
  //   //     fimse
  //   //   fimpara
  //   // fimse
  // }

  // for (int i = 0; i < d->num; i++)
  // {
  //   // se dois elementos do lado esquerdo forem iguais
  //   //   verificar se tem alguem do outro apontando para ele
  //   //   se verdadeiro
  //   //     tirar dependencia
  //   //   fimse
  //   // fimse
  // }
  // for (int i = 0; i < d->num; i++)
  // {
  //   // se dois elementos do lado esquerdo forem iguais e dois elementos do lado direito forem iguais
  //   //   retirar DF's até sobrar apenas uma
  //   // fimse
  // }

  return 0;
}
atributos *keys(struct lista_dependencias *d, int *num_chaves)
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

void normalform(struct lista_dependencias *d)
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
}

/*******************************CODIGO COM PROBLEMAS MUDAR ***************************/

/*************************************************************************************
 ******************************* PUBLIC FUNCTIONS ************************************
 *************************************************************************************/

// void parse(char *path, FDList *list)
// {
//   char *content = read_file(path);
//   int i = 0;
//   int fd_count = 0;
//   char c = content[i];

//   list->size = preComputeLength(content);
//   list->fds = (FD *)malloc(sizeof(FD) * list->size);
//   if (!list->fds)
//   {
//     fprintf(stderr, "ERROR! Couldn't allocate memory for list of functional "
//                     "dependencies\n");
//     exit(1);
//   }

//   while (c != '\0')
//   {
//     if (c == '#') // Pular comentários
//     {
//       while (content[i] != '\n' && content[i] != '\0')
//       {
//         i++;
//       }
//     }
//     else if (c == 'U')
//     {
//       while (content[i] != '{' && content[i] != '\0')
//       {
//         i++;
//       }
//       i++; // Pula '{'

//       while (content[i] != '}' && content[i] != '\0')
//       {
//         if (isInAlphabet(content[i]))
//         {
//           universe[charToIndex(content[i])] = true;
//         }
//         i++;
//       }
//     }
//     else if (c == 'F')
//     {
//       while (content[i] != '{' && content[i] != '\0')
//       {
//         i++;
//       }
//       i++; // Pula '{'

//       while (content[i] != '}' && content[i] != '\0')
//       {
//         attributes lhs = {0};
//         attributes rhs = {0};

// #ifdef _DEBUG_
//         printf("content[i]: %c, %d, %s\n", content[i], __LINE__, __FILE__);
// #endif

//         while ((content[i] != '-' && content[i] != '\0'))
//         {
//           if (isInAlphabet(content[i]))
//           {
//             if (!isInUniverse(content[i]))
//             {
//               fprintf(stderr,
//                       "ERROR! Attribute <%c> doesn't belongs to relation U\n",
//                       content[i]);
//               exit(1);
//             }
//             lhs[charToIndex(content[i])] = true;
//           }
//           i++;
//         }

//         if (content[i] == '-' && content[i + 1] == '>')
//         {
//           i += 2;
//         }

//         while (isspace(content[i]))
//           i++;

// #ifdef _DEBUG_
//         printf("content[i]: %c, line: [%d], at file %s\n", content[i], __LINE__,
//                __FILE__);
// #endif
//         // while ((content[i] != ',' || content[i] == ';') && content[i] != '}'
//         // && content[i] != '\0')
//         while (content[i] != ',' && content[i] != ';' && content[i] != '}' &&
//                content[i] != '\0')
//         {
//           if (isalpha(content[i]) &&
//               (!isInAlphabet(content[i]) || !isInUniverse(content[i])))
//           {
//             fprintf(stderr,
//                     "ERROR! Attribute <%c> doesn't belongs to relation U or "
//                     "doesn't exists\n",
//                     content[i]);
//             exit(1);
//           }
//           rhs[charToIndex(content[i])] = true;
//           i++;
//         }

//         memcpy(&list->fds[fd_count].lhs, lhs, sizeof(attributes));
//         memcpy(&list->fds[fd_count].rhs, rhs, sizeof(attributes));
//         fd_count++;

//         if (content[i] == ',' || content[i] == ';')
//         {
//           i++;
//         }
//       }
//     }

//     i++;
//     c = content[i];
//   }
// }

// void strToAttr(char *str_attr, attributes set)
// {
//   for (size_t i = 0; i < strlen(str_attr); i++)
//   {
//     set[charToIndex(str_attr[i])] = true;
//   }
// }

// char *attrToStr(attributes set)
// {
//   int len = 0;
//   for (int i = 0; i < ALPHABET_SIZE; i++)
//   {
//     if (set[i] == true)
//     {
//       len++;
//     }
//   }

//   char *str = (char *)malloc(len + 1);
//   memset(str, 0, len + 1);

//   len = 0;
//   for (int i = 0; i < ALPHABET_SIZE; i++)
//   {
//     if (set[i] == true)
//     {
//       str[len] = idxToChar(i);
//       len++;
//     }
//   }

//   return str;
// }

// void print_fds(FDList *list)
// {
//   for (size_t i = 0; i < list->size; i++)
//   {
//     FD _fd = list->fds[i];
//     for (int j = 0; j < ALPHABET_SIZE; j++)
//     {
//       if (_fd.lhs[j] == true)
//       {
//         printf("%c", 'A' + j);
//       }
//     }
//     printf("->");
//     for (int j = 0; j < ALPHABET_SIZE; j++)
//     {
//       if (_fd.rhs[j] == true)
//       {
//         printf("%c", 'A' + j);
//       }
//     }
//     printf("\n");
//   }
// }

// /*************************************************************************************
//  ******************************* DEBUG FUNCTIONS *************************************
//  *************************************************************************************/

// void print_debug(FDList *list)
// {
//   for (size_t i = 0; i < list->size; i++)
//   {
//     FD _fd = list->fds[i];
//     printf("lhs\n");
//     for (int i = 0; i < ALPHABET_SIZE; i++)
//     {
//       printf("%c: %s\n", 'A' + i, _fd.lhs[i] == 1 ? "true" : "false");
//     }

//     printf("\nrhs\n");
//     for (int i = 0; i < ALPHABET_SIZE; i++)
//     {
//       printf("%c: %s\n", 'A' + i, _fd.rhs[i] == 1 ? "true" : "false");
//     }
//     printf("--------------------\n");
//   }
// }

// void print_attr_debug(attributes attr)
// {
//   for (int i = 0; i < ALPHABET_SIZE; i++)
//   {
//     if (attr[i])
//     {
//       printf("%c: %s\n", i + 'A', attr[i] == true ? "true" : "false");
//     }
//   }
// }