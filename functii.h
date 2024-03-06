/* STEFAN Alexandru - 315CB */

#include <stdio.h>

typedef struct list {
    int cost;
    int destinatie;
    struct list *next;
} TCelula, *TLista;

typedef struct graf {
    int nr_noduri;
    TLista *x;
}TGraf;

typedef struct punct {
    char nume[50];
    int adancime;
}TPunct;

typedef struct dijkstras {
    int distanta;
    int prev;
    float scor;
}TDijkstras;

TGraf AlocaGraf(int dimensiune);
void DistrugeLista(TLista *l);
void DistrugeGraf(TGraf *graf);
void AdaugaDrum(TGraf graf, int srs, int dest, int cost);
void AdaugaElementLista(TLista *l, int cost, int dest);
int VectorFrecventaPlin(int *vect, int dim);

int NumaraComponenteConexe(TGraf graf, int *vect, int nod);

int CalcCostMin(TGraf graf, int nod_pornire);
int compar(const void *a, const void *b);

void CalcDrumMin(TGraf graf,TDijkstras *drum, TPunct *puncte, int plecare);
int PrintDrumCorabie (TDijkstras *drum, int index, TPunct *puncte, FILE *output);