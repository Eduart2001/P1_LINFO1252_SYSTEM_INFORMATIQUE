#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#define BUFFER_SIZE 8
#define TOTAL_ITEMS 8192 // 8192 elements produits/consommés

void error(int err, char *msg) {
    fprintf(stderr,"%s a retourné %d, message d’erreur : %s\n",msg,err,strerror(errno));
    exit(EXIT_FAILURE);
}

// Initialisation
int buffer[BUFFER_SIZE];
pthread_mutex_t mutex;
sem_t empty, full;
int items_produced = 0;
int items_consumed = 0;

void* producer(void *arg) {
    int item;
    while(items_produced < TOTAL_ITEMS) {
        item=1;
        sem_wait(&empty); // attente d'une place libre
        pthread_mutex_lock(&mutex);
        // section critique
        //printf("items_produced: %d - ", items_produced);
        //printf("[");
        //for (size_t i = 0; i < BUFFER_SIZE-1; i++) printf("%d, ", buffer[i]);
        //printf("%d]\n", buffer[BUFFER_SIZE-1]);
        for (int i = 0;items_produced < TOTAL_ITEMS && i < BUFFER_SIZE; i++) {
            if (buffer[i] == 0){
                buffer[i] = item;
                items_produced++;
                //printf("Producer produces: %d\n", item);
                break;
            }
        }
        pthread_mutex_unlock(&mutex);
        sem_post(&full); // une place remplie en plus
        for (int i=0; i<10000; i++);
    }
    sem_post(&full);
    pthread_exit(NULL);
}

 // Consommateur
void* consumer(void *arg) {
    while(items_consumed < TOTAL_ITEMS) {
        sem_wait(&full); // attente d'une place remplie
        pthread_mutex_lock(&mutex);
        // section critique
        // printf("items_consumed: %d - ", items_consumed);
        // printf("[");
        //for (size_t i = 0; i < BUFFER_SIZE-1; i++) printf("%d, ", buffer[i]);
        //printf("%d]\n", buffer[BUFFER_SIZE-1]);
        for (int i = 0; items_consumed < TOTAL_ITEMS && i < BUFFER_SIZE; i++) {
            if (buffer[i] == 1){
                buffer[i] = 0;
                items_consumed++;
                //printf("Consumer consumes at index: %d\n", i);
                break;
            }
        }
        pthread_mutex_unlock(&mutex);
        sem_post(&empty); // une place libre en plus

        for (int i=0; i<10000; i++);
        //printf("\nConsumed : %d <  Total_items :%d \n", items_consumed ,TOTAL_ITEMS);
    }
    
    sem_post(&empty);

    pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {
    clock_t begin = clock();
    int err;

    err = pthread_mutex_init(&mutex, NULL);
    if(err!=0) error(err,"pthread_mutex_init");

    err = sem_init(&empty, 0 , BUFFER_SIZE); // buffer vide
    if(err!=0) error(err,"sem_init_empty");

    err = sem_init(&full, 0 , 0); // buffer vide
    if(err!=0) error(err,"sem_init_full");

    int producers_number = 0;
    int consumers_number = 0;

    for (int i=0; i<argc-2; i++){
        if (strcmp(argv[i+1], "-P") == 0) producers_number = atol(argv[i+2]);
    }

    for (int i=0; i<argc-2; i++){
        if (strcmp(argv[i+1], "-C") == 0) consumers_number = atol(argv[i+2]);
    }

    if (consumers_number <= 0 || producers_number <= 0) error(err,"thread_numer");

    pthread_t producers[producers_number];
    pthread_t consumers[consumers_number];

    //create
    for(long i=0; i<producers_number; i++) {
        err=pthread_create(&(producers[i]),NULL,&producer,NULL);
        if(err!=0) error(err,"pthread_create_producer");
    }

    for(long i=0; i<consumers_number; i++) {
        err=pthread_create(&(consumers[i]),NULL,&consumer,NULL);
        if(err!=0) error(err,"pthread_create_consumer");
    }
    
    // join
    for(long i=0; i<producers_number; i++) {
        err=pthread_join(producers[i],NULL);
        if(err!=0) error(err,"pthread_join_producer");
    }

    for(long i=0; i<consumers_number; i++) {
        err=pthread_join(consumers[i],NULL);
        if(err!=0) error(err,"pthread_join_consumer");
    }

    sem_destroy(&empty);  
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f\n", time_spent);
    return (EXIT_SUCCESS);
}