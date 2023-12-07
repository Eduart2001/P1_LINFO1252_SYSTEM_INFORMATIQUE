#include <stdio.h>
#include <stdlib.h>

#define N 8
int drapeau[N];
int ticket[N];

void thread(void *arg) {
    int i = *(int *)arg;
    // Thread i
    // Section doorway : annoncer son intérêt et obtenir un ticket
    drapeau[i]=1;
    int t=0;
    // Parcours des tickets
    for (int j=0; j<N; j++) {
        if (ticket[j]>t) {
            t = ticket[j];
        }
    }
    // Prise du ticket supérieur
    ticket[i]=t+1;
    // Section waiting : attendre son tour ...
    int mon_tour;
    do {
        mon_tour = 1;
        // Parcours des tickets des autres threads dont le drapeau est levé
        for (int j=0; j<N; j++) {
            if ((ticket[j] > ticket[i]) || ((ticket[j]==ticket[i]) && j>i)) {
                if (ticket[j] > ticket[i]) {
                    // Il y a un autre thread actif devant dans la file ...
                    mon_tour = 0;
                }
            }
        }
    } while (!mon_tour);
    //section_critique();
    // Libération de threads en attente avec les tickets suivants
    drapeau[i]=0;   
}

int main(int argc, char const *argv[]) {
    // Initialisation
    for (int j=0; j<N; j++) {
    drapeau[j]=0;
    ticket[j]=0;
    }
    return 0;
}