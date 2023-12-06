#define N 8
int level[N];
int victim[N];

void thread(void *arg) {
    int i = *(int *)arg;
    // Thread i
    // Parcours des niveaux 1 à n-1
    for (int L = 1; L < N; L++) {
    // Annoncer l’intention de rentrer au niveau L
    level[i] = L;
    // Le thread se désigne comme la victime pour ce niveau
    victim[L] = i;
    // Attendre tant qu’il existe au moins un thread au même niveau ou à un niveau supérieur,
    // et que le thread i est la victime du niveau où il se trouve
    int t_niv_sup_egal = 0;
    do {
        
        for (int j=0; j< N; j++) {
            // parcours du tableau des niveaux pour déterminer si un thread
            // est au même niveau ou à un niveau supérieur
            if ((j!=i) && level[j] >=L) {
                t_niv_sup_egal = 1;
            }
        }
    } while (t_niv_sup_egal && victim[L]==i);
    }
    // section_critique();
    // Libération de threads bloqués en attente dans les niveaux inférieurs
    level[i]=0;
}

int main(int argc, char const *argv[]) {
    // Initialisation
    for (int i = 0; i < N; i++) {
        level[i] = 0;
    }
    return 0;
}