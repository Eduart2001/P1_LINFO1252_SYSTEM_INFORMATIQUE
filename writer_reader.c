#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>

pthread_mutex_t mutex;
sem_t db; // accès à la db
int readcount=0; // nombre de readers

void writer(void) {
    while(true) {
        prepare_data();
        sem_wait(&db);
        // section critique, un seul writer à la fois
        write_database();
        sem_post(&db);
    }
}

void reader(void) {
    while(true) {
        pthread_mutex_lock(&mutex);
        // section critique
        readcount++;
        if (readcount==1) { // arrivée du premier reader
            sem_wait(&db);
        }
        pthread_mutex_unlock(&mutex);
        read_database();
        pthread_mutex_lock(&mutex);
        // section critique
        readcount--;
        if(readcount==0) { // départ du dernier reader
            sem_post(&db);
        }
        pthread_mutex_unlock(&mutex);
        process_data();
    }
}

int main(int argc, char const *argv[]) {
    sem_init(&db, NULL, 1);
    return 0;
}