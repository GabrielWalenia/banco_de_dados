#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fdtool.h"

int main(int argc, char *argv[])
{
    // if (argc < 2)
    // {
    //     printf("USAGE: ./%s closure --fds <arquivo.fds> --X <ATRIBUTOS>\n",
    //        argv[0]);
    //     printf("USAGE: ./%s mincover --fds <arquivo.fds>\n", argv[0]);
    //     printf("USAGE: ./%s keys --fds <arquivo.fds>\n", argv[0]);
    //     printf("USAGE: ./%s normalform --fds <arquivo.fds>\n", argv[0]);
    //     return 1;
    // }
    struct atributo atr1, atr2, atr3;
    struct relacao u;
    struct dependencia d;

    atr1.attributes[0] = 'A';
    atr1.determina = &atr2;
    atr2.attributes[0] = 'B';
    atr2.determina = &atr3;
    atr3.attributes[0] = 'C';
    atr3.determina = NULL;
    u.r = malloc(3 * sizeof(struct relacao));

    d.num = 2;
    d.lhs = malloc(d.num * sizeof(struct atributo));
    d.rhs = malloc(d.num * sizeof(struct atributo));

    // A no lado esquerdo (1)
    d.lhs[0] = atr1;
    // B no lado esquerdo (2)
    d.lhs[1] = atr2;
    // B no lado direito (1)
    d.rhs[0] = atr2;
    // C no lado esquerdo (2)
    d.rhs[1] = atr3;
    // relacao
    u.r[0] = atr1;
    u.r[1] = atr2;
    u.r[2] = atr3;
    u.num_elementos = 3;

    struct atributo **c = closure(&atr1, &d, 3);
    printf("Chegou ate aqui 2\n");
    printf("o fecho de A é (%c, %c, %c)\n", c[0]->attributes[0], c[1]->attributes[0],
           c[2]->attributes[0]);
    free(u.r);
    return 0;
}
