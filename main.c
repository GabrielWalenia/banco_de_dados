
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fdtool.h"
#include "entrada.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("USAGE: ./%s closure --fds <arquivo.fds> --X <ATRIBUTOS>\n",
               argv[0]);
        printf("USAGE: ./%s mincover --fds <arquivo.fds>\n", argv[0]);
        printf("USAGE: ./%s keys --fds <arquivo.fds>\n", argv[0]);
        printf("USAGE: ./%s normalform --fds <arquivo.fds>\n", argv[0]);
        return 1;
    }

    char *cmd = argv[1];
    char *file_path = argv[3];
    struct lista_dependencias *list;
    FILE *arq = abrir_arquivo(file_path);
    if (!arq)
    {
        fprintf(stderr, "Erro ao abrir arquivo\n");
        exit(1);
    }
    list = gera_lista_dependencias(arq);
    if (!list)
    {
        fprintf(stderr, "Erro ao criar lista\n");
        exit(1);
    }

#ifdef _DEBUG_
    print_debug(&list);
    print_attr_debug(universe);
#endif

    if (strcmp(cmd, "closure") == 0)
    {
        if (argc != 6)
        {
            printf("ERROR! Must have <ATRIBUTOS>\n");
            return 1;
        }

        char *str_attr = argv[5];
        atributos closureSet = {0};
        gerar_atributos(str_attr, closureSet);

        char *fecho = closure(closureSet, list);
        char *str = gerar_strings(fecho);
        printf("%s\n", str);
    }

    return 0;
}
