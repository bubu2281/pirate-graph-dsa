/* STEFAN Alexandru - 315CB */
#include <stdio.h>
#include "functii.h"
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int cerinta;
    //punem in variabila cerinta, argumentul convertit in nr
    cerinta = atoi(argv[1]);
    
    FILE *input, *output;
    //deschidere fisiere input si output
    input = fopen("tema3.in","rt");
    output = fopen("tema3.out", "wt");

    //daca cerinta este 1
    if (cerinta == 1) {
        int nr_obiective, nr_drumuri;

        //citim nr de drumuri si de obiective
        fscanf(input, "%d", &nr_obiective);
        fscanf(input, "%d", &nr_drumuri);

        //vector in care retinem pe pozitia i numele nodului i
        char nume_obiective[nr_obiective + 1][50];
        
        //initializare nume_obiective 
        for (int i = 0; i < nr_obiective + 1; i++) {
            strcpy(nume_obiective[i], "");
        }
        int nr_obiective_curent = 0;

        //alocarea grafului
        TGraf graf = AlocaGraf(nr_obiective);

        //introducere in graf si in vectorul de nume a valorilor citite din input
        for (int i = 0; i < nr_drumuri; i++) {
            char sursa[50], destinatie[50];
            int cost;

            //citirea valorilor
            fscanf(input, "%s", sursa);
            fscanf(input, "%s", destinatie);
            fscanf(input, "%d", &cost);

            //daca  numele sursei se afla deja in vectorul de nume
            //sarim peste, daca nu se afla il introducem
            for (int j = 1; j <= nr_obiective_curent + 1; j++) {
                if (!strcmp(nume_obiective[j], sursa)) {
                    break;
                } else if (j == nr_obiective_curent + 1) {
                    strcpy(nume_obiective[j], sursa);
                    nr_obiective_curent++;
                    break;
                }
            }
            //daca  numele destinatiei se afla deja in vectorul de nume
            //sarim peste, daca nu se afla il introducem
            for (int j = 1; j <= nr_obiective_curent + 1; j++) {
                if (!strcmp(nume_obiective[j], destinatie)) {
                    break;
                } else if (j == nr_obiective_curent + 1) {
                    strcpy(nume_obiective[j], destinatie);
                    nr_obiective_curent++;
                    break;
                }
            }

            //aflarea pozitiei numelui sursei in vect de nume
            //adica aflam ce nod reprezinta sursa
            int poz_sursa = 0;
            for (int j = 0; j < nr_obiective_curent + 1; j++) {
                if(!strcmp(nume_obiective[j],sursa)) {
                    poz_sursa = j;
                    break;
                }
            }

            //la fel ca mai sus, doar ca pentru destinatie
            int poz_destinatie = 0;
            for (int j = 0; j < nr_obiective_curent + 1; j++) {
                if(!strcmp(nume_obiective[j],destinatie)) {
                    poz_destinatie = j;
                    break;
                }
            }

            //adaugarea drumurilor in graf, de la sursa la dest
            //dar si invers deoareve este un graf bidirectional
            AdaugaDrum(graf, poz_sursa, poz_destinatie, cost);
            AdaugaDrum(graf, poz_destinatie, poz_sursa, cost);
            
        }
        //alocare un vector in care retinem pt fiecare componenta conexa
        //costul minim
        int *costuri_minime_comp_conexe = calloc(graf.nr_noduri + 1, sizeof(int));

        //initializarea unui vector de vrecventa
        int vect_frecv[nr_obiective+1];
        for (int i = 0; i <= nr_obiective; i++) {
            vect_frecv[i] = 0;
        }
        int componente_conexe = 0;

        //aplicam algoritmul de parcurgere in adancime 
        //se parcurg doar nodurile nevizitate
        //de de fiecare data cand se parcurge cu succes
        //se apeleaza calculul costului minim pe componenta respectiva
        for (int i = 1; i <= nr_obiective; i++) {
            if (NumaraComponenteConexe(graf, vect_frecv, i)) {
                componente_conexe++;
                costuri_minime_comp_conexe[componente_conexe] = CalcCostMin(graf, i);
            }
        }

        //afisarea numarului de comp conexe
        fprintf(output,"%d\n", componente_conexe);

        //sortarea vectorului de comp conexe si afisarea acestuia
        qsort(costuri_minime_comp_conexe, componente_conexe + 1, sizeof(int), compar);
        for(int i = 1; i <= componente_conexe ; i++) {
            fprintf(output, "%d\n", costuri_minime_comp_conexe[i]);
        }

        //eliberare vect de costuri minime a comp conexe
        free(costuri_minime_comp_conexe);

        //distrugere graf
        DistrugeGraf(&graf);
    }

    //daca cerinta este 2
    if (cerinta == 2) {
        //declaram si citm valorile pentru nr de puncte si de drumuri
        int nr_puncte, nr_drumuri;
        fscanf(input, "%d", &nr_puncte);
        fscanf(input, "%d", &nr_drumuri);
        
        //declarare si initializare puncte
        TPunct puncte[nr_puncte + 1];
        for (int i = 0; i < nr_puncte + 1; i++) {
            strcpy(puncte[i].nume, "");
            puncte[i].adancime = 0;
        }

        // luam nr puncte curent 2 pentru ca vreau ca
        // insula sa fie nodul 1, iar corabia nodul 2
        int nr_puncte_curent = 2;

        //alocare graf
        TGraf graf = AlocaGraf(nr_puncte);

        //citim si adaugam drumurile in graf si in vectorul de nume
        for (int i = 0; i < nr_drumuri; i++) {
            char sursa[50], destinatie[50];
            int cost;
            fscanf(input, "%s", sursa);
            fscanf(input, "%s", destinatie);
            fscanf(input, "%d", &cost);


            //la fel ca la cerinta 1, daca se afla numele sursei deja
            //nu il mai adaugam, daca nu se afla il adaugam, doar ca 
            //vrem ca insula sa fie pe poz 1 si corabia pe poz 2 pentru
            //a ne fi mai usor mai departe
            for (int j = 3; j <= nr_puncte_curent + 1; j++) {
                if (!strcmp("Insula", sursa)) {
                    strcpy(puncte[1].nume, sursa);
                    break;
                }
                if (!strcmp("Corabie", sursa)) {
                    strcpy(puncte[2].nume, sursa);
                    break;
                }
                if (!strcmp(puncte[j].nume, sursa)) {
                    break;
                } else if (j == nr_puncte_curent + 1) {
                    strcpy(puncte[j].nume, sursa);
                    nr_puncte_curent++;
                    break;
                }
            }

            //la fel ca mai sus, doar ca pentru destinatie
            for (int j = 3; j <= nr_puncte_curent + 1; j++) {
                if (!strcmp("Insula", destinatie)) {
                    strcpy(puncte[1].nume, destinatie);
                    break;
                }
                if (!strcmp("Corabie", destinatie)) {
                    strcpy(puncte[2].nume, destinatie);
                    break;
                }
                if (!strcmp(puncte[j].nume, destinatie)) {
                    break;
                } else if (j == nr_puncte_curent + 1) {
                    strcpy(puncte[j].nume, destinatie);
                    nr_puncte_curent++;
                    break;
                }
            }

            //aflarea pozitiei sursei in vect de nume 
            int poz_sursa = 0;
            for (int j = 0; j <= nr_puncte_curent + 1; j++) {
                if(!strcmp(puncte[j].nume,sursa)) {
                    poz_sursa = j;
                    break;
                }
            }

            //aflarea pozitiei destinatiei in bect de nume
            int poz_destinatie = 0;
            for (int j = 0; j <= nr_puncte_curent + 1; j++) {
                if(!strcmp(puncte[j].nume,destinatie)) {
                    poz_destinatie = j;
                    break;
                }
            }

            //adaugarre drum in graf
            AdaugaDrum(graf, poz_sursa, poz_destinatie, cost);
            
        }

        //citirea adancimilor 
        for (int i = 1; i <= nr_puncte; i++) {
            char aux[50];
            //citim numele nodurlui
            fscanf(input, "%s", aux);
            int index;
            //in index retinem nodul
            for (int j = 1; j <= nr_puncte; j++) {
                if (!strcmp(puncte[j].nume, aux)) {
                    index = j;
                }
            }
            //in punctul index citim adancimea
            fscanf(input,"%d",&puncte[index].adancime);

        }
        //declararea si initializarea greutatii corabiei
        int greutate_corabie;
        fscanf(input, "%d", &greutate_corabie);

        //vector ce ne ajuta la dijkstras
        //distanta[i] reprezinta costul drumului cel mai aficient pana la nodul i
        //prev[i] reprezinta nodul anterior din care se ajunge la nodul i
        //scor[i] repreinta scorul minim pentru a ajunge la nodul i
        //initial disanta si scorul le initializam cu nr foarte mari pentru a calc min
        //iar prev -1
        //daca prev == -1 in urma aplicarii algoritmului inseamna ca nodul respectiv
        // ori este chiar de la care se pleaca, ori nu exista drum pana la nodul respectiv
        TDijkstras drum[nr_puncte + 1];
        for (int i = 0; i <= nr_puncte; i++) {
            drum[i].distanta = 999999999;
            drum[i].prev = -1;
            drum[i].scor = 99999999.0;
        }

        //aplicare algoritm dijkstras
        CalcDrumMin(graf, drum, puncte, 2);

        //daca nu se gaseste drum de la corabie la insula
        //se afiseaza un mesaj aferent, se eliberaza memoria
        //si se termina programul
        if (drum[1].prev == -1) {
            fprintf(output, "Echipajul nu poate ajunge la insula\n");
            fclose(input);
            fclose(output);
            DistrugeGraf(&graf);
            return 0;
        }

        //dupa initializam din nou vectorul necesar pt dijkstras
        //pentu a aplica din nou algoritmul de la insula la corabie
        for (int i = 0; i <= nr_puncte; i++) {
            drum[i].distanta = 999999999;
            drum[i].prev = -1;
            drum[i].scor = 9999999.0;
        }

        //aplicare dijkstras de la insula la corabie
        CalcDrumMin(graf, drum, puncte, 1);

        //daca nu exista drum inapoi la corabie se afiseaza mesaj aferent
        //se eliberaza memoria si se iese din main
        if (drum[2].prev == -1) {
            fprintf(output, "Echipajul nu poate transporta comoara inapoi la corabie\n");
            fclose(input);
            fclose(output);
            DistrugeGraf(&graf);
            return 0;
        }

        //afisarea nodurilor ce compun drumul de la insula la corabie
        int adancime_minima = PrintDrumCorabie(drum, 2, puncte, output);
        //afisarea distantei celui mai eficient drum
        fprintf(output,"\n%d\n", drum[2].distanta);
        //afisare adancime minima
        fprintf(output, "%d\n", adancime_minima);
        //calcularea si afisarea numarului de drumuri necesare pentru a 
        //lua toata comoara
        int drumuri_comoara = greutate_corabie/adancime_minima + (greutate_corabie%adancime_minima != 0);
        fprintf(output, "%d\n", drumuri_comoara);
        
        //distrugere graf
        DistrugeGraf(&graf);
    }

    //inchidere fisiere
    fclose(input);
    fclose(output);
    return 0;
}