#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>

#define WRITER_CYCLES 640
#define READER_CYCLES 2560
/* Initialisation */
pthread_mutex_t mutex_readcount; // protège readcount
pthread_mutex_t mutex_writecount; // protège writecount
pthread_mutex_t z; // un seul reader en attente

sem_t wsem; // accès exclusif à la db
sem_t rsem; // pour bloquer des readers

int readcount=0;
int writecount=0;

int total_reads = 0;
int total_writes = 0;

void error(int err, char *msg) {
    fprintf(stderr,"%s a retourné %d, message d’erreur : %s\n",msg,err,strerror(errno));
    exit(EXIT_FAILURE);
}

/* Writer */
void  *writer (void *arg){
    for(int i =0;i<*((int*)arg);++i) {
        pthread_mutex_lock(&mutex_writecount);
        total_writes++;
        printf("total writes: %d\n", total_writes);
        // section critique - writecount
        
        writecount++;
        if(writecount==1) {
    	    // premier writer arrive
            sem_wait(&rsem);
        }
        pthread_mutex_unlock(&mutex_writecount);
        sem_wait(&wsem);// section critique, un seul writer à la fois
        
        for (int i=0; i<10000; i++);
        sem_post(&wsem);
        pthread_mutex_lock(&mutex_writecount);
        // section critique - writecount
        writecount--;
        if(writecount==0) {
            // départ du dernier writer
            sem_post(&rsem);
        }
        pthread_mutex_unlock(&mutex_writecount);
    }
    pthread_exit(NULL);
}

/* Reader */
void *reader(void *arg){
    for(int i =0;i<*((int*)arg);++i) {
        pthread_mutex_lock(&z);
        // exclusion mutuelle, un seul reader en attente sur rsem
        sem_wait(&rsem);
        pthread_mutex_lock(&mutex_readcount);
        total_reads++;
        // printf("total reads: %d\n", total_reads);
        // exclusion mutuelle, readercount
        readcount++;
        if (readcount==1) {
            // arrivée du premier reader
            sem_wait(&wsem);
        }

        pthread_mutex_unlock(&mutex_readcount);
        sem_post(&rsem); // libération prochain reader/writer
        
        pthread_mutex_unlock(&z);
        for (int i=0; i<10000; i++);
        
        pthread_mutex_lock(&mutex_readcount);
        // exclusion mutuelle, readcount
        readcount--;
        if(readcount==0) {
            // départ du dernier reader
            sem_post(&wsem);
        }
        pthread_mutex_unlock(&mutex_readcount);
    }
    pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {
    int reader_number, writer_number;

    for (int i=0; i<argc-2; i++){
        if (strcmp(argv[i+1], "-R") == 0) reader_number = atoi(argv[i+2]);
    }

    for (int i=0; i<argc-2; i++){
        if (strcmp(argv[i+1], "-W") == 0) writer_number = atoi(argv[i+2]);
    }
    int err;

    err = pthread_mutex_init(&mutex_readcount, NULL);
    if(err!=0) error(err,"pthread_mutex_init readcount");

    err = pthread_mutex_init(&mutex_writecount, NULL);
    if(err!=0) error(err,"pthread_mutex_init writecount");

    err = pthread_mutex_init(&z, NULL);
    if(err!=0) error(err,"pthread_mutex_init only one reader");

    err = sem_init(&wsem, 0 , 1); // buffer vide
    if(err!=0) error(err,"sem_init_empty wsem");

    err = sem_init(&rsem, 0 , 1); // buffer vide
    if(err!=0) error(err,"sem_init_full rsem");

    pthread_t reader_thread[reader_number];
    pthread_t writer_thread[writer_number];

    for (int i = 0; i < writer_number; i++) {
        if (i == writer_number-1) {
            int cycles_last_writer = (WRITER_CYCLES/writer_number) + (WRITER_CYCLES%writer_number);
            pthread_create(&writer_thread[i], NULL, &writer, (void *) &(cycles_last_writer));
        }
        else {
            int cycles_per_writer = WRITER_CYCLES/writer_number;
            pthread_create(&writer_thread[i], NULL, &writer, (void *) &(cycles_per_writer));
        }
    }

    for (int i = 0; i < reader_number; i++) {
        if (i == reader_number-1){
            int cycles_last_reader = (READER_CYCLES/reader_number) + (READER_CYCLES%reader_number);
            pthread_create(&reader_thread[i], NULL, &reader, (void *) &(cycles_last_reader));
        }
        else {
            int cycles_per_reader = READER_CYCLES/reader_number;
            pthread_create(&reader_thread[i], NULL, &reader, (void *) &(cycles_per_reader));
        }
    }

    // writer: join and clean up
    for (int i = 0; i < writer_number; ++i) {
        int error = pthread_join(writer_thread[i], NULL);
        if (error != 0) {
            perror("Failed to join writer thread");
            exit(EXIT_FAILURE);
        }
    }

    // reader: join and clean up
    for (int i = 0; i < reader_number; ++i) {
        int error = pthread_join(reader_thread[i], NULL);
        if (error != 0) {
            perror("Failed to join reader thread");
            exit(EXIT_FAILURE);
        }
    }

    pthread_mutex_destroy(&mutex_readcount);
    pthread_mutex_destroy(&mutex_writecount);
    pthread_mutex_destroy(&z);
    sem_destroy(&wsem);
    sem_destroy(&rsem);
    printf("total reads: %d, total writes: %d\n", total_reads, total_writes);
    // printf("READER_CYCLES: %d, WRITER_CYCLES: %d\n", READER_CYCLES, WRITER_CYCLES);
    return (EXIT_SUCCESS);
}