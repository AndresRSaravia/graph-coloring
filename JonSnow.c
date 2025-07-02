#include "JonSnow.h" 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>


//AUXILIARES
int cmpfunc(const void * a, const void * b) {
    return ( *(u32*)a - *(u32*)b );
}

u32 BS(u32 *array, u32 elem, u32 left, u32 right) {

    u32 l = left;
    u32 r = right;
    u32 middle = 0;
    while(l<=r)
    {
        middle = (l+r)/2;
        if(elem>array[middle]) {
            l = middle+1;
        } else if(elem < array[middle]) {
            r = middle-1;
        } else {
            break;
        }
    }
    return middle;
}

//Intercambio en order
void swap_order(WinterIsHere W, u32 i, u32 j) {
    u32 tmp;
    tmp = W->order[i];
    W->order[i] = W->order[j];
    W->order[j] = tmp;
}

//Función Pivote Aleatorio y Pivote Fijo
u32 pivot(WinterIsHere W, u32 izq, u32 der) {
    u32 piv;
    u32 i;
    u32 j;
    
    piv = (u32) ((float)((der-izq + 1))*rand()/(RAND_MAX+1.0)) + izq;
    swap_order(W,izq,piv);
    
    piv = izq;
    i = izq + 1;
    j = der;
    while (i<=j) {
        if (W->degree[i] > W->degree[piv]) {
            i = i+1;
        }
        else if (W->degree[j] <= W->degree[piv]) {
            j = j-1;
        }
        else {
			      swap_order(W,i,j);
			      i = i+1;
			      j = j-1;
        }
    }
    swap_order(W,piv,j);
    piv = j;
    return piv;
}

//Rand_Quick_s Recursión
void welsh_powell_sort_rec(WinterIsHere W, u32 izq, u32 der) {
    u32 piv;
    if (der>izq) {
        piv = pivot(W,izq,der);
    	  welsh_powell_sort_rec(W,izq,piv-1);
        welsh_powell_sort_rec(W,piv+1,der);
    }
}

//Rand_Quick_s Principal
void welsh_powell_sort(WinterIsHere W, u32 length) {
	if (length!=0) {
		welsh_powell_sort_rec(W,0,length-1);
	}
}



//PRINCIPALES
WinterIsHere WinterIsComing() {
//En caso de error, la función devolverá un puntero a NULL.
//Errores posibles pueden ser falla en alocar memoria, pero
//también que el formato de entrada no sea válido.

    printf("¡Bienvenido al prototipo de main de Saravia y Hormann Manriquez!\n");
    printf("Ingrese el nombre del archivo para ver la magia: ");
    char file_name[25];
    scanf("%s", file_name);
    printf("FASE 0: LECTURA DE ARCHIVO\n");
    
    
    FILE *fp;
    fp = fopen(file_name,"r");
    if( fp == NULL )
    {
        perror("Archivo no encontrado.\n");
        exit(EXIT_FAILURE);
    }
    printf("DONE.\n");
    printf("FASE 1: INICIALIZACiÓN Y REGISTRO DE ARISTAS\n");
    WinterIsHere W = NULL;
    W = calloc(1, sizeof(struct WinterSt));
    char str1[20];
    char str2[20];
    char *tmp1 = "p";
    char *tmp2 = "edge";
    u32 A, B;
    /*printf("Contenido de %s:\n", file_name);*/
    do{
        fscanf(fp, "%s %s %u %u\n", str1, str2, &A, &B);
        /*printf("%s %s %u %u\n", str1, str2, A, B);*/
    }while ((strcmp(str1, tmp1))!=0||(strcmp(str2, tmp2))!=0);
    W->n_vertexes = A;
    W->n_edges = B;
    u32 *all = calloc(2*B, sizeof(u32));
    //all porque es mas representativo, estan todos los real_value del input.
    u32 *column1 = calloc(B, sizeof(u32));
    //column1 porque se corresponde con los inputs de la primer columna de longitud B. 
    u32 *column2 = calloc(2*B, sizeof(u32));
    //column2 porque: idem al column1
    u32 e1, e2;
    for(u32 i=0;i<B;i++) {
        fscanf(fp, "%s %u %u\n", str1, &e1, &e2);
        /*printf("%s %u %u\n", str1, e1, e2);*/
        if (i%1000000==0) {
            printf("iteración number %u\n", i);
        }
        all[2*i] = e1;
        all[2*i+1] = e2;
        column1[i] = e1;
        column2[i] = e2;
    }
    fclose(fp);
    /*for (u32 i=0; i<2*B; i++) {
        printf("%u ", all[i]);
    }
    printf("\n");*/
    qsort(all, 2*B, 4, cmpfunc);
    /*for (u32 i=0;i<2*B; i++) {
        printf("%u ", all[i]);
    }
    printf("\n");*/
    printf("DONE.\n");
    printf("FASE 2: ASIGNACIÓN DE ETIQUETAS Y PRIMERAS REORGANIZACIONES\n");
    
    printf("Aloja memoria\n");
    printf("A: %u\n", A);
    u32 *counter = calloc(A, sizeof(u32));
    printf("size of counter %lu\n", sizeof(*counter));
    //counter porque cuenta la frecuencia de las apariciones de cada real_value perteneciente a "all"
    W->vertex = calloc(A, sizeof(u32));
    printf("size of vertex %lu\n", sizeof(W->vertex));
    W->degree = calloc(A, sizeof(u32));
    printf("size of degree %lu\n", sizeof(W->degree));
    W->order = calloc(A, sizeof(u32));
    printf("size of order %lu\n", sizeof(W->order));
    u32 tag = 0;
    u32 max_degree = 0;
    u32 cant = 1;
    u32 real_value = all[0];
    printf("Empezamos el ciclo\n");
    for (u32 i=1; i<2*B; i++) {
        if (i>24985000&&24985100>i&&(i%100==0)) {
            printf("iteración number %u\n", i);
        }
        if(all[i]!=real_value) {
            //printf("Comienzo encuentro con distinto\n");
            counter[tag] = cant;
            W->degree[tag] = cant;
            if(cant>=max_degree) {
                max_degree = cant;
            }
            W->vertex[tag] = real_value;
            W->order[tag] = tag;
            cant = 1;
            real_value = all[i];
            tag++;
        } else {
            cant++;
        }
        if(i==2*B-1) {
            printf("C 2*B-1\n");
            counter[tag] = cant;
            W->degree[tag] = cant;
            W->vertex[tag] = real_value;
            W->order[tag] = tag;
            printf("F 2*B-1\n");
        }
    }




    /*for (u32 i=0; i<A; i++) {
        printf("|%u %u|\n", i, W->vertex[i]);
    }*/
    printf("DONE.\n");
    printf("FASE 3: CREACIÓN DE LA LISTA DE ADYACENCIA\n");
    W->adj = calloc((A), (sizeof(u32*)));
    for (u32 tag=0; tag<A; tag++) {
        printf("let's see");
        W->adj[tag] = calloc((W->degree[tag]), (sizeof(u32)));
    }

    u32 tag_e1, tag_e2;
    printf("mmm\n");
    for (u32 i=0; i<B; i++) {
        //printf("Antes\n");
        tag_e1 = BS(W->vertex, column1[i], 0, (A-1));
        tag_e2 = BS(W->vertex, column2[i], 0, (A-1));
        //printf("entre\n");
        //printf("tag1 %u, tag2 %u, degree1 %u, counter1 %u, degree2 %u, counter2 %u\n", tag_e1, tag_e2, W->degree[tag_e1], counter[tag_e1], W->degree[tag_e2], counter[tag_e2]);
        //printf("c1\n");
        if(W->degree[tag_e1]!=0) {
            W->adj[tag_e1][W->degree[tag_e1]-counter[tag_e1]] = tag_e2;
            counter[tag_e1]--;
        }
        //printf("c2\n");
        if(W->degree[tag_e2]!=0) {
            W->adj[tag_e2][W->degree[tag_e2]-counter[tag_e2]] = tag_e1;
            counter[tag_e2]--;
        }
        //printf("Después\n");
    }
    printf("good\n");
    /*for (u32 i=0; i<A; i++) {
        printf("|%u %u|", i, W->vertex[i]);
        for (u32 j=0; j<W->degree[i]; j++) {
            printf("%u", W->adj[i][j]);
        }
        printf("\n");
    }*/
    printf("DONE.\n");
    printf("FASE 4: RECOLECCIÓN DE BASURA\n");
    
    //Liberar counter, all, column1 y column2.
    free(counter);
    free(all);
    free(column1);
    free(column2);
    
    counter = NULL;
    all = NULL;
    column1 = NULL;
    column2 = NULL;

    printf("DONE.\n");
    printf("FASE 5: APLICACIÓN GREEDY\n");

    W->max_degree = max_degree;
    W->colour = calloc(A, sizeof(u32));
    W->n_v_icolour = calloc(max_degree+2, sizeof(u32));
    W->n_colours = Greedy(W);

    /*printf("Max_degree: %u\n", max_degree);
    for (u32 i=0; i<A; i++) {
        printf("%u ", i);
    }
    printf("<-Tags\n");

    for (u32 i=0; i<A; i++) {
        printf("%u ", W->degree[i]);
    }
    printf("<-Grados\n");
    for (u32 i=0; i<A; i++) {
        printf("%u ", W->vertex[i]);
    }
    printf("<-Vértices\n");
    for (u32 i=0; i<A; i++) {
        printf("%u ", W->order[i]);
    }
    printf("<-Orden\n");

    for (u32 i=0; i<A; i++) {
        printf("%u ", W->colour[i]);
    }
    printf("<-Coloreo\n");

    for (u32 i=0; i<W->max_degree+1; i++) {
        printf("%u ", W->n_v_icolour[i]);
    }
    printf("<-N° de vértices de i color\n");*/
    printf("Coloreo: %u\n", W->n_colours);
    printf("DONE...?\n");
    return W;
}

int Primavera(WinterIsHere W) {
//Destruye W y libera la memoria alocada. Retorna 1 si todo anduvo bien y 0 si no.
    //printf("free de info\n");
    u32 n_vertex = W->n_vertexes;
    free(W->vertex);
    free(W->degree);
    free(W->order);
    free(W->colour);
    free(W->n_v_icolour);
    //printf("free de adyacencia\n");
    for(u32 i=0; i<n_vertex; i++) {
        //printf("Veamos\n");
        free(W->adj[i]);
        W->adj[i] = NULL;
    }
    //printf("pasó\n");
    free(W->adj);
    W->vertex = NULL;
    W->degree = NULL;
    W->order = NULL;
    W->colour = NULL;
    W->n_v_icolour = NULL;
    free(W);
    W = NULL;
    return 1;
    //Revisar 'assert' para el return 0
}


u32 NombreDelVertice(WinterIsHere W, u32 x) {
    u32 o = W->vertex[x];
    return o;
}

u32 ColorDelVertice(WinterIsHere W, u32 x) {
    u32 o = W->colour[x];
    return o;
}

u32 GradoDelVertice(WinterIsHere W, u32 x) {
    u32 o = W->degree[x];
    return o;
}

u32 IesimoVecino(WinterIsHere W, u32 x, u32 i) {
    u32 o = W->adj[x][i];
    return o;
}

u32 NumeroDeVertices(WinterIsHere W) {
    u32 o = W->n_vertexes;
    return o;
}

u32 NumeroDeLados(WinterIsHere W) {
    u32 o = W->n_edges;
    return o;
}

u32 NumeroVerticesDeColor(WinterIsHere W,u32 i) {
    u32 o = W->n_v_icolour[i];
    return o;
}

u32 NumeroDeColores(WinterIsHere W) {
    u32 o = W->n_colours;
    return o;
}

u32 IesimoVerticeEnElOrden(WinterIsHere W,u32 i) {
    u32 o = W->order[i];
    return o;
}

void OrdenNatural(WinterIsHere W) {
    for (u32 i=0; i<W->n_vertexes; i++) {
        W->order[i] = i;
    }
//Investigar formas eficientes(05042017-23:37---no encontré, pero tampoco busqué mucho en profundidad)
//17042017-17:42 Nikov, es inevitable que sea lineal.
}


void OrdenWelshPowell(WinterIsHere W) {
    //Quiero ordenar el arreglo order según sus valencias de manera decreciente.
    welsh_powell_sort(W, W->n_vertexes);
}
/*
void AleatorizarVertices(WinterIsHere W,u32 x) {
    srand(time(NULL));
    u32 r = rand();
}

void ReordenManteniendoBloqueColores(WinterIsHere W,u32 x) {
    //Investigar aleatorio
}*/

u32 Greedy(WinterIsHere W) {
    u32 *colour_used = calloc(W->max_degree+2, sizeof(u32));
    u32 tag, result, tag_colour;
    for(u32 tag=0;tag<W->n_vertexes;tag++) {
        W->colour[tag] = 0;
    }
    for(u32 j=1;j<(W->max_degree)+2;j++) {
        colour_used[j] = 0;
        W->n_v_icolour[j] = 0;//n_v_icolour
    }
    result = 1;
    for(u32 j=0;j<W->n_vertexes;j++) {
        tag = W->order[j];
        for(u32 i=0;i<W->degree[tag];i++) {
            colour_used[W->colour[IesimoVecino(W, tag, i)]] = 1;
        }
        tag_colour = 1;
        while(colour_used[tag_colour]!=0) {
            tag_colour++;
        }
        for(u32 i=0;i<W->degree[tag];i++) {
            colour_used[W->colour[IesimoVecino(W, tag, i)]] = 0;
        }
        W->colour[tag] = tag_colour;
        W->n_v_icolour[tag_colour]++;//n_v_icolour
        if(tag_colour>result) {
            result = tag_colour;
        }
    }
    free(colour_used);
    colour_used = NULL;
    return result;
}

int Bipartito(WinterIsHere W) {
    u32 *colour_used = calloc(3, sizeof(u32));
    u32 *colour_tmp = calloc(W->n_vertexes, sizeof(u32));
    u32 *n_v_icolour_tmp = calloc(3, sizeof(u32));
    
    u32 tag, tag_colour;
    bool b;
    
    for(u32 tag=0;tag<W->n_vertexes;tag++) {
        colour_tmp[tag] = 0;
    }
    for(u32 j=1;j<3;j++) {
        colour_used[j] = 0;
        n_v_icolour_tmp[j] = 0;
    }
    
    b = true;
    
    u32 j = 0;
    while(j<W->n_vertexes&&(b)) {
        tag = W->order[j];
        for(u32 i=0;i<W->degree[tag];i++) {
            colour_used[colour_tmp[IesimoVecino(W, tag, i)]] = 1;
        }
        
        tag_colour = 1;
        
        while(colour_used[tag_colour]!=0&&b) {
            if(tag_colour==2) {
                b = false;
            }
            tag_colour++;
        }
        
        if(b) {
            for(u32 i=0;i<W->degree[tag];i++) {
                colour_used[colour_tmp[IesimoVecino(W, tag, i)]] = 0;
            }
            colour_tmp[tag] = tag_colour;
            n_v_icolour_tmp[tag_colour]++;
        }
        j++;
    }

    if (b) {
        free(W->colour);
        free(W->n_v_icolour);
        W->colour = colour_tmp;
        W->n_v_icolour = n_v_icolour_tmp;
        W->n_colours = 2;
        colour_tmp = NULL;
        n_v_icolour_tmp = NULL;
        colour_used = NULL;
        return 1;
    }
    else {
        free(colour_tmp);
        free(n_v_icolour_tmp);
        free(colour_used);
        colour_tmp = NULL;
        n_v_icolour_tmp = NULL;
        colour_used = NULL;
        return 0;
    }
}
