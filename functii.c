/* STEFAN Alexandru - 315CB */
#include "functii.h"
#include <stdlib.h>
#include <stdio.h>


//functie de alocare graf
TGraf AlocaGraf(int dimensiune) {
    TGraf g;
    g.nr_noduri = dimensiune;
    g.x = (TLista *)malloc(sizeof(TLista)*(dimensiune + 1));
    for (int i = 0; i <= dimensiune; i++) {
        (g.x)[i] = NULL;
    }
    return g;
}

//functie de distrugere a listei
void DistrugeLista(TLista *l) {
    TLista p = *l;
    TLista ant = *l;
    while(p) {
        p = p->next;
        free(ant);
        ant = p;
    }
    free(ant);
    *l = NULL;
}

//functie de distrugere a grafului
void DistrugeGraf(TGraf *graf) {
    for (int i = 1; i <= graf->nr_noduri; i++) {
        DistrugeLista(&((graf->x)[i]));
    }
    graf->nr_noduri = 0;
    free(graf->x);
    return;
}

//functie ce adauga drum in graf
void AdaugaDrum(TGraf graf, int srs, int dest, int cost) {
    AdaugaElementLista(&((graf.x)[srs]), cost, dest);
}

//functie ce adauga un element in lista, la finalul ei
void AdaugaElementLista(TLista *l, int cost, int dest) {
    TLista aux = (TLista)malloc(sizeof(TCelula));
    aux->cost = cost;
    aux->destinatie = dest;
    aux->next = NULL;
    if (*l == NULL) {
        *l = aux;
        return;
    }
    TLista p = *l;
    TLista ant = p;
    while(p) {
        ant = p;
        p = p->next;
    }
        ant->next = aux;
        return;
}


//functie ce verifica daca un vector de frecventa este plin
//functie nefolosita in tema
//la un mom dat m-am folosit de ea, dar in final am renuntat la
//ea
int VectorFrecventaPlin(int *vect, int dim) {
    for (int i = 1; i <= dim; i++) {
        if (vect[i] == 0) {
            return 0;
        }
    }
    return 1;
}

//functie ce parcurge in adancime un graf doar pe nodurile
//nevizitate
int NumaraComponenteConexe(TGraf graf, int *vect, int nod) {
    TLista lista_adiacenta = graf.x[nod];
    TLista aux = lista_adiacenta;
    if(vect[nod] == 1) {
        return 0;
    }
    vect[nod] = 1;
    while (aux) {
        int dest = aux->destinatie;
        if(vect[dest] == 0) {
            NumaraComponenteConexe(graf, vect, dest);
        }
        aux = aux->next;
    }
    return 1;
}

//aplicare alogritm Prim
int CalcCostMin(TGraf graf, int nod_pornire) {
    int vect_frecv[graf.nr_noduri + 1];
    for (int i = 1; i <= graf.nr_noduri; i++) {
        vect_frecv[i] = 0;
    }
    vect_frecv[nod_pornire] = 1;



    int cost = 0;
    for(int j = 1; j <= graf.nr_noduri; j++) {
        struct min {
            int min;
            int nod;
        } min_curent;

        min_curent.min = 99999;
        min_curent.nod = 0;
        for (int i = 1; i <= graf.nr_noduri; i++) {
            if (vect_frecv[i] == 1) {
                TLista p = graf.x[i];
                while(p) {
                    if(vect_frecv[p->destinatie] == 0 || min_curent.nod == p->destinatie) {
                        if(min_curent.min > p->cost) {
                            min_curent.min = p->cost;
                            min_curent.nod = p->destinatie;
                        }
                    }
                    p = p->next;
                }
            }
        }
        if (min_curent.min < 10000) {
            vect_frecv[min_curent.nod] = 1;
            cost += min_curent.min;
        }
    }
    return cost;
}

//functie de compareare, folosita pentru qsort
int compar(const void *a, const void *b) {
    return *((int *)a) - *((int *)b);
}


//aplicare algoritm Dijkstras in functie de scor
void CalcDrumMin(TGraf graf,TDijkstras *drum, TPunct *puncte, int plecare) {
    int vizitat[graf.nr_noduri];
    for (int i = 0; i <= graf.nr_noduri; i++) {
        vizitat[i] = 0;
    }
    drum[plecare].distanta = 0;
    drum[plecare].scor = 0.0;
    for (int j = 1; j <= graf.nr_noduri; j++) {
        int index = 0;
        for (int i = 1; i <= graf.nr_noduri; i++) {
            if ((drum[index].scor > drum[i].scor) && (vizitat[i] == 0)) {
                index = i;
            }
        }

        if (vizitat[index] == 0 ) {
            TLista p = graf.x[index];
            while (p) {
                if (vizitat[p->destinatie] == 0) {
                    float scor_curent = ((float)p->cost)/puncte[p->destinatie].adancime;
                    if ((drum[index].scor + scor_curent) < drum[p->destinatie].scor) {
                        drum[p->destinatie].scor = drum[index].scor + scor_curent;
                        drum[p->destinatie].distanta = drum[index].distanta + p->cost;
                        drum[p->destinatie].prev = index;
                    }
                }
                p = p->next;
            }
            vizitat[index] = 1;
        }
    }   
}


//functie ce printeaza drumul dupa aplicare algoritmului
//dijkstras de la sursa la index
int PrintDrumCorabie (TDijkstras *drum, int index, TPunct *puncte, FILE *output) {
    int adancime_minima;
    if (index != 1 && index != 2) {
        adancime_minima = puncte[index].adancime;
    } else {
        adancime_minima = 9999999;
    }
    if (drum[index].prev == -1) {
        fprintf(output,"%s ", puncte[index].nume);
        return adancime_minima;
    }
    int aux = PrintDrumCorabie(drum , drum[index].prev, puncte, output);
    if (aux < adancime_minima) {
        adancime_minima = aux;
    }
    fprintf(output,"%s ", puncte[index].nume);
    return adancime_minima;
}