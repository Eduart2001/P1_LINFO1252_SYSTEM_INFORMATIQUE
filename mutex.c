#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include "error.h"
#define NTHREADS 4

long global=0;
pthread_mutex_t mutex_global;

int increment(int i) {
    return i+1;
}

void *func(void * param) {
    int err;
    for(int j=0;j<1000000;j++) {
        err=pthread_mutex_lock(&mutex_global);
        if(err!=0)
        error(err,"pthread_mutex_lock");
        global=increment(global);
        err=pthread_mutex_unlock(&mutex_global);
        if(err!=0)
        error(err,"pthread_mutex_unlock");
    }
    return(NULL);
}

int main (int argc, char *argv[]) {
    pthread_t thread[NTHREADS];
    int err;
    err=pthread_mutex_init(&mutex_global, NULL);
    if(err!=0) error(err,"pthread_mutex_init");
    for(int i=0;i<NTHREADS;i++) {
        err=pthread_create(&(thread[i]),NULL,&func,NULL);
        if(err!=0)
        error(err,"pthread_create");
    }
    for(int i=0; i<1000000000;i++) { /*...*/ }
    for(int i=NTHREADS-1;i>=0;i--) {
        err=pthread_join(thread[i],NULL);
        if(err!=0)
        error(err,"pthread_join");
    }
    err=pthread_mutex_destroy(&mutex_global);
    if(err!=0) error(err,"pthread_mutex_destroy");
    printf("global: %ld\n",global);
    return(EXIT_SUCCESS);
}