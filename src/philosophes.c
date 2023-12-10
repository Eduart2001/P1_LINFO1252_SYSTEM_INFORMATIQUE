#include "../headers/error.h"

#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

pthread_t *phil;
pthread_mutex_t *baguette;
long philosophes = 0;
int verbose = false;

void* philosophe ( void* arg ) {
    int *id=(int *) arg;
    int left = *id;
    int right = (left + 1) % philosophes;
    for(int i=0; i<1000000; i++) {
        if (verbose) printf("Philosopher [%d] thinks\n",*id);
        if (left < right) {
            if(pthread_mutex_lock(&baguette[left])) error("pthread_mutex_lock left baguette");
            if(pthread_mutex_lock(&baguette[right])) error("pthread_mutex_lock right baguette");
        } else {
            if(pthread_mutex_lock(&baguette[right])) error("pthread_mutex_lock right baguette");
            if(pthread_mutex_lock(&baguette[left])) error("pthread_mutex_lock left baguette");
        }
        if (verbose) {
            printf("Philosopher [%d] takes the baguette on his left [%d]\n",*id,left);
            printf("Philosopher [%d] takes the baguette on his right [%d]\n",*id,right);
            printf("Philosopher [%d] eats\n",*id);
        }
        if(pthread_mutex_unlock(&baguette[left])) error("pthread_mutex_unlock left baguette");
        if (verbose) printf("Philosopher [%d] releases his left baguette [%d]\n",*id,left);
        if(pthread_mutex_unlock(&baguette[right])) error("pthread_mutex_unlock right baguette");
        if (verbose) printf("Philosopher [%d] releases his right baguette [%d]\n",*id,right);
    }
    pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {
    clock_t begin = clock();
    for (int i=0; i<argc-2; i++){
        if (strcmp(argv[i+1], "-N") == 0) philosophes = atol(argv[i+2]);
        else if (strcmp(argv[i+1], "-v") == 0) verbose = true;
    }
    if (strcmp(argv[argc-1], "-v") == 0) verbose = true;

    if (philosophes <= 1) {
        printf("Please provide a thread number bigger than 1\n");
        return (EXIT_FAILURE);
    }

    int arg[philosophes];

    phil = (pthread_t *)malloc(philosophes*sizeof(pthread_t));
    if (phil == NULL) error("phil malloc failed");
    
    baguette = (pthread_mutex_t *)malloc(philosophes*sizeof(pthread_mutex_t));
    if (baguette == NULL) error("baguette malloc failed");

    for(long i=0; i<philosophes; i++) {
        arg[i]=i;
        if(pthread_mutex_init(&(baguette[i]), NULL)) error("pthread_mutex_init");
        if(pthread_create(&(phil[i]),NULL,&philosophe,(void *) &(arg[i]))) error("pthread_create");
    }

    for(int i=0;i<philosophes;i++) {
        if(pthread_join(phil[i],NULL)) error("pthread_join");
    }

    for(int i=0;i<philosophes;i++) {
        if(pthread_mutex_destroy(&baguette[i])) error("mutex destroy");
    }

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f\n", time_spent);

    return (EXIT_SUCCESS);
}