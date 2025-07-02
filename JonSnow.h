/*
   Hormann Manriquez, Nicolas Andrés-nicolashtml2012@gmail.com
   Saravia, Andrés Román-------------andresromansaravia@gmail.com
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>
typedef unsigned int u32;
struct t_truple {
    u32 x;
    u32 y;
    u32 z;
};
typedef struct t_truple truple;

struct t_tuple {
  u32 x;
  u32 y;
};
typedef struct t_tuple tuple;

struct WinterSt {

    u32 n_vertexes;
    u32 n_edges;
    u32 n_colours;
    u32 max_degree;

    u32 *vertex;
    u32 **adj;
    u32 *degree;

    u32 *order;
    u32 *colour;
    u32 *n_v_icolour;
    u32 *colour_used;
    truple *copy_tcn;
    tuple *copy_dv;
};
typedef struct WinterSt *WinterIsHere;

//Funciones auxiliares
int cmpfunc (const void * a, const void * b);
u32 BS(u32 *array, u32 elem, u32 left, u32 right);

//Funciones principales
WinterIsHere WinterIsComing();
//En caso de error, la función devolverá un puntero a NULL.
//Errores posibles pueden ser falla en alocar memoria, pero
//también que el formato de entrada no sea válido.


int Primavera(WinterIsHere W);
//Destruye W y libera la memoria alocada. Retorna 1 si todo anduvo bien y 0 si no.

u32 NombreDelVertice(WinterIsHere W, u32 x);
u32 ColorDelVertice(WinterIsHere W, u32 x);
u32 GradoDelVertice(WinterIsHere W, u32 x);
u32 IesimoVecino(WinterIsHere W, u32 x,u32 i);
 
void OrdenNatural(WinterIsHere W);
void OrdenWelshPowell(WinterIsHere W);
void AleatorizarVertices(WinterIsHere W,u32 x);
void ReordenManteniendoBloqueColores(WinterIsHere W,u32 x);

u32 Greedy(WinterIsHere W);
int Bipartito(WinterIsHere W);

u32 NumeroDeVertices(WinterIsHere W);
u32 NumeroDeLados(WinterIsHere W);
u32 NumeroVerticesDeColor(WinterIsHere W,u32 i);
u32 NumeroDeColores(WinterIsHere W);
u32 IesimoVerticeEnElOrden(WinterIsHere W,u32 i);
