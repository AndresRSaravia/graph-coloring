#include <stdio.h>
#include <stdlib.h>
#include "JonSnow.h"
#include <string.h>
#include <assert.h>

int main()
{
    printf("¡Bienvenido al prototipo de main de Saravia y Hormann Manriquez!\n");
    WinterIsHere F = WinterIsComing();
    printf("El grafo ha sido creado\n");

    printf("\n");
    OrdenWelshPowell(F);
    printf("\n");
    for (u32 i=0; i<F->n_vertexes; i++) {
        if (1<i) {
            assert(F->degree[F->order[i-1]]>=F->degree[F->order[i]]);
        }
    }

    printf("\n");
    Primavera(F);
    printf("El grafo ha sido destruido\n");
    return 0;
}
