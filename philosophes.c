#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "error.h"

pthread_t *phil;
pthread_mutex_t *baguette;
long philosophes = 0;

void* philosophe ( void* arg ) {
    int *id=(int *) arg;
    int left = *id;
    int right = (left + 1) % philosophes;
    for(int i=0; i<10000000; i++) {
        //printf("Philosophe [%d] pense\n",*id);
        if (left < right) {
            pthread_mutex_lock(&baguette[left]);
            pthread_mutex_lock(&baguette[right]);
        } else {
            pthread_mutex_lock(&baguette[right]);
            pthread_mutex_lock(&baguette[left]);
        }
        printf("Philosophe [%d] possède baguette gauche [%d]\n",*id,left);
        printf("Philosophe [%d] possède baguette droite [%d]\n",*id,right);
        //printf("Philosophe [%d] mange\n",*id);
        pthread_mutex_unlock(&baguette[left]);
        printf("Philosophe [%d] a libéré baguette gauche [%d]\n",*id,left);
        pthread_mutex_unlock(&baguette[right]);
        printf("Philosophe [%d] a libéré baguette droite [%d]\n",*id,right);
        }
    return (NULL);
}

int main(int argc, char const *argv[]) {
    int err;
    for (int i=0; i<argc-1; i++){
        if (strcmp(argv[i+1], "-N") == 0) philosophes = atol(argv[i+2]);
        //philosophes = strtol(argv[i+2], &endptr, 10);
    }
    // printf("%ld\n", philosophes);
    // return (EXIT_SUCCESS);
    if (philosophes < 0) error(err,"thread_numer");
    int arg[philosophes];
    phil = (pthread_t *)malloc(philosophes*sizeof(pthread_t));
    baguette = (pthread_mutex_t *)malloc(philosophes*sizeof(pthread_mutex_t));
    for(long i=0; i<philosophes; i++) {
        arg[i]=i;
        err = pthread_mutex_init(&(baguette[i]), NULL);
        if(err!=0) error(err,"pthread_mutex_init");
        err=pthread_create(&(phil[i]),NULL,&philosophe,(void *) &(arg[i]));
        if(err!=0) error(err,"pthread_create");
    }
    for(int i=0;i<philosophes;i++) {
        err=pthread_join(phil[i],NULL);
        if(err!=0) error(err,"pthread_join");
    }
    free(phil);
    free(baguette);
    return (EXIT_SUCCESS);
}