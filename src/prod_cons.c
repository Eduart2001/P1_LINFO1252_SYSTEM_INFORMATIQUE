#include "../headers/error.h"

#include <semaphore.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

#define BUFFER_SIZE 8
#define TOTAL_ITEMS 8192 // 8192 produced/consumed elements

// Initialization
int buffer[BUFFER_SIZE];
pthread_mutex_t mutex;
sem_t empty, full;
int items_produced = 0;
int items_consumed = 0;
int verbose = false;

void* producer(void *arg) {
    int item;
    while(items_produced < TOTAL_ITEMS) {
        item=1;
        if(sem_wait(&empty)) error("producer initial sem_wait failed"); // Waits for a free place
        if(pthread_mutex_lock(&mutex)) error("mutex_lock failed");
        // critical section
        if (verbose) {
            printf("items_produced: %d - ", items_produced);
            printf("[");
            for (size_t i = 0; i < BUFFER_SIZE-1; i++) printf("%d, ", buffer[i]);
            printf("%d]\n", buffer[BUFFER_SIZE-1]);
        }

        for (int i = 0;items_produced < TOTAL_ITEMS && i < BUFFER_SIZE; i++) {
            if (buffer[i] == 0){
                buffer[i] = item;
                items_produced++;
                if(verbose) printf("Producer produces: %d\n", item);
                break;
            }
        }
        if(pthread_mutex_unlock(&mutex)) error("mutex_unlock failed");
        if(sem_post(&full)) error("sem_post failed"); // Another filled place
        for (int i=0; i<10000; i++); // hard work
    }
    if(sem_post(&full)) error("producer sem_post at the end failed");
    pthread_exit(NULL);
}

void* consumer(void *arg) {
    while(items_consumed < TOTAL_ITEMS) {
        if(sem_wait(&full)) error("consumer initial sem_wait failed"); // Waits for a free place
        if(pthread_mutex_lock(&mutex)) error("mutex_lock failed");
        // critical section
        if(verbose) {
            printf("items_consumed: %d - ", items_consumed);
            printf("[");
            for (size_t i = 0; i < BUFFER_SIZE-1; i++) printf("%d, ", buffer[i]);
            printf("%d]\n", buffer[BUFFER_SIZE-1]);
        }
        
        for (int i = 0; items_consumed < TOTAL_ITEMS && i < BUFFER_SIZE; i++) {
            if (buffer[i] == 1){
                buffer[i] = 0;
                items_consumed++;
                if(verbose) printf("Consumer consumes at index: %d\n", i);
                break;
            }
        }
        if(pthread_mutex_unlock(&mutex)) error("mutex_unlock failed");
        if(sem_post(&empty)) error("sem_post failed"); // Another filled place
        for (int i=0; i<10000; i++); // hard work
    }
    if(sem_post(&empty)) error("consumer sem_post at the end failed");
    pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {
    clock_t begin = clock();

    if(pthread_mutex_init(&mutex, NULL)) error("pthread_mutex_init");

    if(sem_init(&empty, 0 , BUFFER_SIZE)) error("sem_init_empty"); // empty buffer

    if(sem_init(&full, 0 , 0)) error("sem_init_full");

    int producers_number = 0;
    int consumers_number = 0;

    for (int i=0; i<argc-2; i++){
        if (strcmp(argv[i+1], "-P") == 0) producers_number = atol(argv[i+2]);
        else if (strcmp(argv[i+1], "-C") == 0) consumers_number = atol(argv[i+2]);
        else if (strcmp(argv[i+1], "-v") == 0) verbose = true;
    }
    if (strcmp(argv[argc-1], "-v") == 0) verbose = true;

    if (consumers_number <= 0 || producers_number <= 0) error("Please provide a valid consumer or producer number of threads");

    pthread_t producers[producers_number];
    pthread_t consumers[consumers_number];

    // create
    for(long i=0; i<producers_number; i++) {
        if(pthread_create(&(producers[i]),NULL,&producer,NULL)) error("pthread_create_producer");
        if(pthread_create(&(consumers[i]),NULL,&consumer,NULL)) error("pthread_create_consumer");
    }
    
    // join
    for(long i=0; i<producers_number; i++) {
        if(pthread_join(producers[i],NULL)) error("pthread_join_producer");
        if(pthread_join(consumers[i],NULL)) error("pthread_join_consumer");
    }

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f\n", time_spent);
    return (EXIT_SUCCESS);
}