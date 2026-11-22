#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "entrada.h"
#include "fdtool.h"

#define charToIndex(c) ((c) - 'A')
#define idxToChar(idx) ((idx) + 'A')

char alfabeto[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

bool isInUniverse(char c)
{
    return universo[charToIndex(c)];
}

bool isInAlphabet(char c)
{
    int len = strlen(alfabeto);
    for (int i = 0; i < len; i++)
    {
        if (c == alfabeto[i])
        {
            return true;
        }
    }
    return false;
}
FILE *abrir_arquivo(char *path)
{
    if (!path)
    {
        fprintf(stderr, "Erro! Caminho não especificado\n");
        return NULL;
    }

    FILE *arquivo = fopen(path, "rb");

    if (!arquivo)
    {
        fprintf(stderr, "Erro! Não foi possível abrir o arquivo\n");
        return NULL;
    }

    return arquivo;
}
int calcular_tamanho(char *conteudo)
{
    int num = 0;
    char *primeira_ocorrencia = strchr(conteudo, 'F');
    if (!primeira_ocorrencia)
    {
        fprintf(stderr, "Erro ao achar inicio das dependencias funcionais\n");
        return -1;
    }

    while (primeira_ocorrencia)
    {
        char *aux = primeira_ocorrencia + 1;
        while (*aux == ' ')
        {
            aux++;
        }

        if (*aux == '=')
        {
            break;
        }

        primeira_ocorrencia = strchr(primeira_ocorrencia + 1, 'F');
    }

    while (*primeira_ocorrencia && *primeira_ocorrencia != '}')
    {
        if (primeira_ocorrencia[0] == '-' && primeira_ocorrencia[1] == '>')
        {
            num++;
            primeira_ocorrencia += 2;
        }
        else
        {
            primeira_ocorrencia++;
        }
    }

    return num;
}

void fechar_arquivo(FILE *arquivo)
{
    if (!arquivo)
    {
        fprintf(stderr, "Arquivo não especificado");
        return;
    }
    fclose(arquivo);
}
char *le_arquivo(FILE *arquivo)
{
    if (!arquivo)
    {
        fprintf(stderr, "Arquivo não especificado\n");
        return NULL;
    }

    fseek(arquivo, 0, SEEK_END);
    long tamanho_arquivo = ftell(arquivo);
    rewind(arquivo);

    tamanho_arquivo = tamanho_arquivo + 1;

    // Lembrar de fechar
    char *conteudo = (char *)malloc(tamanho_arquivo);
    if (!conteudo)
    {
        fprintf(stderr, "falha ao alocar memória para o conteudo\n");
        return NULL;
    }
    size_t bytes_lidos = fread(conteudo, 1, tamanho_arquivo, arquivo);

    conteudo[bytes_lidos] = '\0';
    fclose(arquivo);
    return conteudo;
}

struct lista_dependencias *gera_lista_dependencias(FILE *arq)
{
    if (!arq)
    {
        fprintf(stderr, "Arquivo não especificado");
        return NULL;
    }

    char *conteudo = le_arquivo(arq);
    if (!conteudo)
    {
        fprintf(stderr, "Não foi possível extrair o conteudo do arquivo\n");
        fclose(arq);
        return NULL;
    }

    // Não esquecer de liberar espaço depois
    struct lista_dependencias *l =
        (struct lista_dependencias *)malloc(sizeof(struct lista_dependencias));
    if (!l)
    {
        fprintf(stderr, "Erro ao alocar lista de dependências!\n");
        free(conteudo);
        return NULL;
    }
    l->size = calcular_tamanho(conteudo);
    // Lembrar de fechar
    l->dep = (struct dependencia *)malloc(sizeof(struct dependencia) * l->size);

    if (!l->dep)
    {
        fprintf(stderr, "Erro ao alocar memória para lista de dependencias!\n");
        free(conteudo);
        free(l);
        return NULL;
    }

    int i = 0;
    char caractere = conteudo[i];
    int num_dependencias = 0;

    while (caractere != '\0')
    {

        if (caractere == 'U')
        {
            while (conteudo[i] != '{' && conteudo[i] != '\0')
            {
                i++;
            }
            i++;

            while (conteudo[i] != '}' && conteudo[i] != '\0')
            {
                if (isInAlphabet(conteudo[i]))
                {
                    universo[charToIndex(conteudo[i])] = 1;
                }
                i++;
            }
        }

        else if (caractere == 'F')
        {
            while (conteudo[i] != '{' && conteudo[i] != '\0')
            {
                i++;
            }
            i++;

            while (conteudo[i] != '}' && conteudo[i] != '\0')
            {
                atributos lhs = {0};
                atributos rhs = {0};

                while ((conteudo[i] != '-' && conteudo[i] != '\0'))
                {
                    if (isInAlphabet(conteudo[i]))
                    {
                        if (!isInUniverse(conteudo[i]))
                        {
                            fprintf(stderr, "ERRO");
                            return NULL;
                        }
                        lhs[charToIndex(conteudo[i])] = 1;
                    }
                    i++;
                }

                if (conteudo[i] == '-' && conteudo[i + 1] == '>')
                {
                    i += 2;
                }

                while (isspace(conteudo[i]))
                    i++;

                while (conteudo[i] != ',' && conteudo[i] != ';' && conteudo[i] != '}' &&
                       conteudo[i] != '\0')
                {
                    if (isalpha(conteudo[i]) &&
                        (!isInAlphabet(conteudo[i]) || !isInUniverse(conteudo[i])))
                    {
                        fprintf(stderr,
                                "Falha! Atributo não pertence a relação ou não existe\n");
                        exit(1);
                    }
                    rhs[charToIndex(conteudo[i])] = 1;
                    i++;
                }
                memcpy(&l->dep[num_dependencias].lhs, lhs, sizeof(atributos));
                memcpy(&l->dep[num_dependencias].rhs, rhs, sizeof(atributos));
                num_dependencias++;

                if (conteudo[i] == ',' || conteudo[i] == ';')
                {
                    i++;
                }
            }
        }

        i++;
        caractere = conteudo[i];
    }
    free(conteudo);
    return l;
}

void gerar_atributos(char *buffer, atributos atr)
{
    for (int i = 0; i < (int)strlen(buffer); i++)
    {
        atr[charToIndex(buffer[i])] = 1;
    }
}

char *gerar_strings(atributos atr)
{
    int len = 0;
    for (int i = 0; i < TAM_ALFABETO; i++)
    {
        if (atr[i] == 1)
        {
            len++;
        }
    }
    // Lembrar de fechar
    char *str = (char *)malloc(len + 1);
    memset(str, 0, len + 1);

    len = 0;
    for (int i = 0; i < TAM_ALFABETO; i++)
    {
        if (atr[i] == true)
        {
            str[len] = idxToChar(i);
            len++;
        }
    }
    return str;
}
void imprimir_fds(struct lista_dependencias *list)
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

// Debugar
void print_debug(struct lista_dependencias *list)
{
    for (int i = 0; i < list->size; i++)
    {
        struct dependencia _fd = list->dep[i];
        printf("lhs\n");
        for (int i = 0; i < TAM_ALFABETO; i++)
        {
            printf("%c: %s\n", 'A' + i, _fd.lhs[i] == 1 ? "true" : "false");
        }

        printf("\nrhs\n");
        for (int i = 0; i < TAM_ALFABETO; i++)
        {
            printf("%c: %s\n", 'A' + i, _fd.rhs[i] == 1 ? "true" : "false");
        }
        printf("--------------------\n");
    }
}
void print_attr_debug(atributos attr)
{
    for (int i = 0; i < TAM_ALFABETO; i++)
    {
        if (attr[i])
        {
            printf("%c: %s\n", i + 'A', attr[i] == true ? "true" : "false");
        }
    }
}