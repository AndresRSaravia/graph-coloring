#include "JonSnow.h"
#include <time.h>
//#include <assert.h>

//AUXILIARES
//PARA EL ARREGLO ALL
int cmpall(const void * a, const void * b) {
    u32 r = *(u32*)a;
    u32 s = *(u32*)b;
    if(r>s) {
        return 1;
    }
    else if(r==s){
        return 0;
    }
    else {
        return -1;
    }
}

//PARA WELSH 
int cmpWP(const void *a, const void *b){
    tuple *r = (tuple *) a;
    tuple *s = (tuple *) b;
    if(r->x<s->x) {
        return 1;
    }
    else if(r->x == s->x) {
        return 0;
    }
    else {
        return -1;
    }
}

int cmpRMBC01(const void *a, const void *b){
    //RMBC01 = Reorden Manteniendo Bloque Colores para x=0 y x=1
    truple *r = (truple *) a;
    truple *s = (truple *) b;
    if (r->y > s->y) {
        return 1;
    }
    else if (r->y == s->y) {
        return 0;
    }
    else {
        return -1;
    }
}

int cmpRMBC2(const void *a, const void *b){
    //RMBC01 = Reorden Manteniendo Bloque Colores para x=2
    truple *r = (truple *) a;
    truple *s = (truple *) b;
    if (r->z != s->z) {
        if(r->z > s->z) {
            return 1;
        }
        else {
            return -1;
        }
    }
    else {
        if(r->y > s->y) {
            return 1;
        }
        else if (r->y < s->y) {
            return -1;
        }
        else {
            return 0;
        }
    }
}

int cmpRMBC34(const void *a, const void *b){
    //RMBC01 = Reorden Manteniendo Bloque Colores para x>=3
    truple *r = (truple *) a;
    truple *s = (truple *) b;

    if (r->z != s->z) {
        if(r->z > s->z) {
            return -1;
        }
        else {
            return 1;
        }
    }
    else {
        if(r->y > s->y) {
            return 1;
        }
        else if (r->y < s->y) {
            return -1;
        }
        else {
            return 0;
        }
    }
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
        } else if(elem<array[middle]) {
            r = middle-1;
        } else {
            break;
        }
    }
    return middle;
}

void swap(u32 *a, u32 i, u32 j) {
    u32 tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}

//Funcion de aleatorizar
void yat(u32 *a, u32 length, u32 x) {
    srand(x+1);
    
    for (u32 k=0;k<length;k++) {
        u32 t = rand()%(length);
        swap(a,t,k);
    }
}

//PRINCIPALES
WinterIsHere WinterIsComing() {
    //INICIALIZACION Y REGISTRO DE ARISTAS
    //clock_t t = clock();
    WinterIsHere W = NULL;
    W = calloc(1, sizeof(struct WinterSt));
    if(W == NULL) {
        printf("Error al asignarle espacio de memoria a W\n");
        return NULL;
    }
    char str1[20];
    char str2[20];
    char *tmp1 = "p";
    char *tmp2 = "edge";
    u32 A, B;
    
    int cant_scanf = scanf("%s %s %u %u\n", str1, str2, &A, &B);
    while ((strcmp(str1, tmp1))!=0||(strcmp(str2, tmp2))!=0) {
		cant_scanf = scanf("%s %s %u %u\n", str1, str2, &A, &B);
    }
    if(cant_scanf != 4)
      return NULL;
    W->n_vertexes = A;
    W->n_edges = B;
    u32 *all = calloc(2*B, sizeof(u32));//metemos todos los vertices que aparecen en las B aristas...osea (2 por B) apariciones de vertices
    if(all == NULL) {
        printf("Error al asignar espacio de memoria.\n");
        return NULL;
    }
    u32 *column1 = calloc(B, sizeof(u32));//vertices que aparecen en la primer columna del input
    if(column1 == NULL) {
        printf("Error al asignar espacio de memoria.\n");
        return NULL;
    }
    u32 *column2 = calloc(2*B, sizeof(u32));//vertices que aparecen en la segunda columna del input
    if(column2 == NULL) {
        printf("Error al asignar espacio de memoria.\n");
        return NULL;
    }
    u32 e1, e2;//vertice e1 representa cada vertice que va a ir recorriendo en la column1 y e2 respecto a column2
    for(u32 i=0;i<B;i++) {
        cant_scanf = scanf("%s %u %u\n", str1, &e1, &e2);
        all[2*i] = e1;
        all[2*i+1] = e2;
        column1[i] = e1;
        column2[i] = e2;
    }
    printf("Cantidad de vertices: %u\n", A);
    printf("Cantidad de aristas: %u\n", B);
    qsort(all, 2*B, sizeof(u32), cmpall);//ordenamos el arreglo all, de menor a mayor para hacer binary search
    //ASIGNACION DE ETIQUETAS Y PRIMERAS REORGANIZACIONES
    u32 *counter = calloc(A, sizeof(u32));//contador de frecuencias de cada vertice
    if(counter == NULL) {
        printf("Error al asignar espacio de memoria.\n");
        return NULL;
    }
    W->vertex = calloc(A, sizeof(u32));
    if(W->vertex == NULL) {
        printf("Error al asignar espacio de memoria.\n");
        return NULL;
    }
    W->degree = calloc(A, sizeof(u32));
    if(W->degree == NULL) {
        printf("Error al asignar espacio de memoria.\n");
        return NULL;
    }
    W->order = calloc(A, sizeof(u32));
    if(W->order == NULL) {
        printf("Error al asignar espacio de memoria.\n");
        return NULL;
    }
    u32 tag = 0;
    u32 max_degree = 0;
    u32 cant = 1;
    u32 real_value = all[0];
    for(u32 i=1;i<2*B;i++) {
        if(all[i]!=real_value) {
            counter[tag] = cant;//se va a usar para los vecinos y dimensionar el arreglo de vecinos, de cada vertice
            W->degree[tag] = cant;
            if(cant>=max_degree) {
                max_degree = cant;
            }
            W->vertex[tag] = real_value;//dejamos guardado el valor real
            W->order[tag] = tag;//ya seteamos el orden natural desde este momento
            cant = 1;
            real_value = all[i];//pasamos al siguiente valor real
            tag++;
        } else {
            cant++;
        }
        if(i==2*B-1) {//esto cubre la ultima iteracion
            counter[tag] = cant;
            W->degree[tag] = cant;
            W->vertex[tag] = real_value;
            W->order[tag] = tag;
        }
    }
    //CREACION DE LA LISTA DE ADYACENCIA
    W->adj = calloc((A), (sizeof(u32*)));
    if(W->adj == NULL) {
        printf("Error al asignar espacio de memoria.\n");
        return NULL;
    }
    for(u32 tag=0;tag<A;tag++) {
        W->adj[tag] = calloc((W->degree[tag]), (sizeof(u32)));
        if(W->adj[tag] == NULL) {
        	printf("Error al asignar espacio de memoria.\n");
    		return NULL;
    	}
    }
    u32 tag_e1, tag_e2;
    for(u32 i=0;i<B;i++) {
        tag_e1 = BS(W->vertex, column1[i], 0, (A-1));
        tag_e2 = BS(W->vertex, column2[i], 0, (A-1));
        if(counter[tag_e1]!=0) {
            W->adj[tag_e1][W->degree[tag_e1]-counter[tag_e1]] = tag_e2;
            counter[tag_e1]--;
        }
        if(counter[tag_e2]!=0) {
            W->adj[tag_e2][W->degree[tag_e2]-counter[tag_e2]] = tag_e1;
            counter[tag_e2]--;
        }
    }
    //RECOLECCION DE BASURA
    free(counter);
    free(all);
    free(column1);
    free(column2);
    counter = NULL;
    all = NULL;
    column1 = NULL;
    column2 = NULL;
    
    W->max_degree = max_degree;
    W->colour = calloc(A, sizeof(u32));
    if(W->colour == NULL) {
        printf("Error al asignar espacio de memoria.\n");
        return NULL;
    }
    W->n_v_icolour = calloc(max_degree+2, sizeof(u32));
    if(W->n_v_icolour == NULL) {
        printf("Error al asignar espacio de memoria.\n");
        return NULL;
    }
    W->colour_used = calloc(max_degree+2, sizeof(u32));
    if(W->colour_used == NULL) {
        printf("Error al asignar espacio de memoria.\n");
        return NULL;
    }
    W->copy_dv = calloc(W->n_vertexes, sizeof(tuple));
    if(W->copy_dv == NULL) {
        printf("Error al asignar espacio de memoria.\n");
        return NULL;
    }
	W->copy_tcn = calloc(W->n_vertexes, sizeof(truple));
	if(W->copy_tcn == NULL) {
        printf("Error al asignar espacio de memoria.\n");
        return NULL;
    }
    for(u32 j=1;j<max_degree+2;j++) {
		W->colour_used[j] = 0;
    }
    //printf("Carga del grafo:%f\n", (double)(clock()-t)/CLOCKS_PER_SEC);
    W->n_colours = Greedy(W);
    return W;
}

int Primavera(WinterIsHere W) {
    if(W!=NULL&&W->vertex!=NULL&&W->degree!=NULL&&W->order!=NULL&&W->colour!=NULL
      &&W->n_v_icolour&&W->adj!=NULL && W->colour_used != NULL && W->copy_dv!=NULL && W->copy_tcn!=NULL) {
        u32 n_vertex = W->n_vertexes;
        free(W->vertex);
        free(W->degree);
        free(W->order);
        free(W->colour);
        free(W->n_v_icolour);
        free(W->colour_used);
        for(u32 i=0;i<n_vertex;i++) {
            if(W->adj[i]!=NULL) {
                free(W->adj[i]);
                W->adj[i] = NULL;
            }
        }
        free(W->copy_dv);
        free(W->copy_tcn);
        free(W->adj);
        W->vertex = NULL;
        W->degree = NULL;
        W->order = NULL;
        W->colour = NULL;
        W->n_v_icolour = NULL;
        W->colour_used = NULL;
        W->copy_dv = NULL;
        W->copy_tcn = NULL;
        W->adj = NULL;
        free(W);
        W = NULL;
        return 1;
    }
    return 0;
}


u32 NombreDelVertice(WinterIsHere W, u32 x) {
    u32 real_value = 0;
    if(x < W->n_vertexes) {
       real_value = W->vertex[x];
    }
    return real_value;
}

u32 ColorDelVertice(WinterIsHere W, u32 x) {
    u32 colour = 0;
    if(x < W->n_vertexes) {
       colour = W->colour[x];
    }
    return colour;
}

u32 GradoDelVertice(WinterIsHere W, u32 x) {
    u32 degree = 0;
    if(x < W->n_vertexes) {
       degree = W->degree[x];
    }
    return degree;
}

u32 IesimoVecino(WinterIsHere W, u32 x, u32 i) {
    u32 iesimo_vecino = 0;
    if(x < W->n_vertexes) {
       iesimo_vecino = W->adj[x][i];
    }
    return iesimo_vecino;
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
    u32 cant_colour_i = 0;
    if(i <= W->max_degree + 1) {
        cant_colour_i = W->n_v_icolour[i];
    }
    return cant_colour_i;
}

u32 NumeroDeColores(WinterIsHere W) {
    u32 o = W->n_colours;
    return o;
}

u32 IesimoVerticeEnElOrden(WinterIsHere W,u32 i) {
    u32 tag = 0;
    if(i < W->n_vertexes) {
       tag = W->order[i];
    }
    return tag;
}

void OrdenNatural(WinterIsHere W) {
    for(u32 i=0;i<W->n_vertexes;i++) {
        W->order[i] = i;
    }
}

void OrdenWelshPowell(WinterIsHere W) {
    //clock_t t = clock();
    for(u32 i = 0;i<W->n_vertexes;i++) {
        W->copy_dv[i].y = W->order[i];
        W->copy_dv[i].x = W->degree[W->order[i]];
    }
    //Quiero ordenar el arreglo order según sus grados de manera decreciente.
    qsort(W->copy_dv, W->n_vertexes, sizeof(tuple), cmpWP);
    for(u32 i=0;i<W->n_vertexes;i++) {
        W->order[i] = W->copy_dv[i].y;
    }
    //printf("Welsh:%f\n", (double)(clock()-t)/CLOCKS_PER_SEC);
}

void AleatorizarVertices(WinterIsHere W,u32 x) {
    //clock_t t = clock();
    OrdenNatural(W);
    yat(W->order, W->n_vertexes, x);
    //printf("Aleatorizar vertices:%f\n", (double)(clock()-t)/CLOCKS_PER_SEC);
}

void ReordenManteniendoBloqueColores(WinterIsHere W,u32 x) {
	//tcn = tag, color, número de vértices por color
	//clock_t t = clock();
	u32 selected_colour;
	for(u32 i=0;i<W->n_vertexes;i++) {
	    W->copy_tcn[i].x = W->order[i];
	    W->copy_tcn[i].y = W->colour[W->order[i]];
	    W->copy_tcn[i].z = W->n_v_icolour[W->colour[W->order[i]]];
	}
	if (x==0) {
	    for (u32 i = 0; i<W->n_vertexes; i++) {
	        selected_colour = W->copy_tcn[i].y;
	        if(selected_colour!=W->n_colours) {
	        	W->copy_tcn[i].y = W->copy_tcn[i].y+1;
	        }
	        else {
	        	W->copy_tcn[i].y = 1;
	        }
	    }
	    qsort(W->copy_tcn, W->n_vertexes, sizeof(truple), cmpRMBC01);
	}
	else if (x==1) {
	    for (u32 i = 0; i<W->n_vertexes; i++) {
	        selected_colour = W->copy_tcn[i].y;
	        W->copy_tcn[i].y = W->n_colours - selected_colour;
	    }
	    qsort(W->copy_tcn, W->n_vertexes, sizeof(truple), cmpRMBC01);
	}
	else if (x==2) {
	    qsort(W->copy_tcn, W->n_vertexes, sizeof(truple), cmpRMBC2);
	}
	else if (x==3) {
	    qsort(W->copy_tcn, W->n_vertexes, sizeof(truple), cmpRMBC34);
	}
	else {
	    u32 *permutation = calloc(W->n_colours, sizeof(u32));
	    for(u32 i=0; i<W->n_colours; i++) {
	        permutation[i] = i+1;
	    }
	    yat(permutation, W->n_colours, x);
	    for(u32 i=0; i<W->n_vertexes; i++) {
	        W->copy_tcn[i].y = permutation[W->copy_tcn[i].y-1];
	    }
	    qsort(W->copy_tcn, W->n_vertexes, sizeof(truple), cmpRMBC34);
	    free(permutation);
	    permutation = NULL;
	}
	
	for(u32 i=0; i<W->n_vertexes; i++) {
	    W->order[i] = W->copy_tcn[i].x;
	}
	//printf("Reorden bloques:%f\n", (double)(clock()-t)/CLOCKS_PER_SEC);
}

u32 Greedy(WinterIsHere W) {
    clock_t t = clock();
    u32 tag, result, tag_colour;
    for(u32 tag=0;tag<W->n_vertexes;tag++) {
        W->colour[tag] = 0;
    }
    
    for(u32 j=1;j<(W->max_degree)+2;j++) {
        W->n_v_icolour[j] = 0;//n_v_icolour
    }
    result = 1;
    for(u32 j=0;j<W->n_vertexes;j++) {
        tag = W->order[j];
        for(u32 i=0;i<W->degree[tag];i++) {
          /*if(W->colour[W->adj[tag][i]] <= result) {
              W->colour_used[W->colour[W->adj[tag][i]]] = 1;
            }*/
            W->colour_used[W->colour[W->adj[tag][i]]] = 1;
        }
        tag_colour = 1;
        while(W->colour_used[tag_colour]!=0) {//Encontramos el primer 0, y asignamos el color
            tag_colour++;
        }
        for(u32 i=0;i<W->degree[tag];i++) {
            W->colour_used[W->colour[W->adj[tag][i]]] = 0;
        }
        W->colour[tag] = tag_colour;
        W->n_v_icolour[tag_colour]++;//n_v_icolour
        if(tag_colour>result) {
            result = tag_colour;
        }
    }
    W->n_colours = result;
    //printf("Coloreo:%u\n", result);
    printf("Greedy:%f\n", (double)(clock()-t)/CLOCKS_PER_SEC);
    return result;
}

int Bipartito(WinterIsHere W) {
    u32 *visited = calloc(W->n_vertexes,sizeof(u32));
    u32 *colour_tmp = calloc(W->n_vertexes,sizeof(u32));
    u32 *n_v_icolour_tmp = calloc(3,sizeof(u32));
   
    for(u32 i=0;i<W->n_vertexes;i++) {
        visited[i] = 0;
    }

    u32 *queue_bfs = calloc(W->n_vertexes,sizeof(u32));
    queue_bfs[0] = 0;
    visited[0] = 1;
    colour_tmp[0] = 1;
    n_v_icolour_tmp[1] = 1;
    u32 next_position = 1;
    u32 actual_position = 0;
    while(actual_position!=W->n_vertexes) {
        if(actual_position==next_position) {
            u32 i = 0;
            while(i<W->n_vertexes && visited[i]==1) {
                i++;
            }
            if (visited[i]==0) {
                queue_bfs[actual_position] = i;
                visited[i] = 1;
                colour_tmp[i] = 1;
                next_position++;
            }
        }
        u32 selected = queue_bfs[actual_position];
        for(u32 j=0;j<W->degree[selected];j++) {
            u32 u = W->adj[selected][j];
            if (visited[u] == 0) {
                visited[u] = 1;
                colour_tmp[u] = 3 - colour_tmp[selected];
                n_v_icolour_tmp[colour_tmp[u]]++;
                queue_bfs[next_position] = u;
                next_position++;
            }
        }
        actual_position++;
    }

    u32 proper = 1;
    for(u32 i = 0; i < W->n_vertexes; i++){
        u32 c = colour_tmp[i];
        for(u32 j = 0; j < W->degree[i]; j++){
            u32 v = W->adj[i][j];
            if(c == colour_tmp[v])
                proper = 0;
        }
    }
    if(proper) {
        for(u32 i = 0;i<W->n_vertexes;i++)
        {
          W->colour[i] = colour_tmp[i];
        }
        W->n_v_icolour[1] = n_v_icolour_tmp[1];
        W->n_v_icolour[2] = n_v_icolour_tmp[2];
        W->n_colours = 2;
        free(colour_tmp);
        free(n_v_icolour_tmp);
        free(visited);        
        free(queue_bfs);
        visited = NULL;
        colour_tmp = NULL;
        n_v_icolour_tmp = NULL;
        queue_bfs = NULL;
        return 1;
    }
    else {
        free(colour_tmp);
        free(n_v_icolour_tmp);
        free(visited);        
        free(queue_bfs);
        visited = NULL;
        colour_tmp = NULL;
        n_v_icolour_tmp = NULL;
        queue_bfs = NULL;
        return 0;
    }
}
