#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>

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

void write_database(){

    for (int i = 0; i < 10000; i++) {
        
    }
}

void read_database(){

    for (int i = 0; i < 10000; i++) {
        
    }
}

/* Writer */
void  *writer (void *arg){
    int* n_writer = (int*) arg;
    for(int i =0;i<READER_CYCLES/(*n_writer);++i)
    {
        printf("Writer #%d is preparing data\n", );
        //think_up_data();
        pthread_mutex_lock(&mutex_writecount);
        // section critique - writecount
        
        writecount=writecount+1;
        if(writecount==1) {
    	    // premier writer arrive
            sem_wait(&rsem);
        }
        pthread_mutex_unlock(&mutex_writecount);
        sem_wait(&wsem);// section critique, un seul writer à la fois
        
        write_database();
        sem_post(&wsem);
        pthread_mutex_lock(&mutex_writecount);
        // section critique - writecount
        writecount=writecount-1;
        if(writecount==0) {
            // départ du dernier writer
            sem_post(&rsem);
        }
        pthread_mutex_unlock(&mutex_writecount);
    }


}

/* Reader */
void *reader(void *arg){
    int* n_reader = (int*) arg;
    for(int i =0;i<READER_CYCLES/(*n_reader);++i)
    {
        pthread_mutex_lock(&z);
        // exclusion mutuelle, un seul reader en attente sur rsem
        sem_wait(&rsem);
        pthread_mutex_lock(&mutex_readcount);
        // exclusion mutuelle, readercount
        readcount++;
        if (readcount==1) {
            printf("Reader #%d is the first reader, waiting on wsem\n", i);
            // arrivée du premier reader
            sem_wait(&wsem);
        }

        pthread_mutex_unlock(&mutex_readcount);
        sem_post(&rsem); // libération prochain reader/writer
        
        pthread_mutex_unlock(&z);
        printf("Reader #%d is reading\n", i);
        read_database();
        
        pthread_mutex_lock(&mutex_readcount);
        // exclusion mutuelle, readcount
        readcount--;
        if(readcount==0) {
            // départ du dernier reader
            printf("Reader #%d was the last reader\n", i);
            sem_post(&wsem);
        }
        pthread_mutex_unlock(&mutex_readcount);
        printf("Reader #%d is processing data read\n", i);
        //use_data_read();
    }
}

int main(int argc, char const *argv[])
{
    int reader_number, writer_number;

    for (int i=0; i<argc-2; i++){
        if (strcmp(argv[i+1], "-R") == 0) reader_number = atol(argv[i+2]);
    }

    for (int i=0; i<argc-2; i++){
        if (strcmp(argv[i+1], "-W") == 0) writer_number = atol(argv[i+2]);
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
    pthread_t z;


    semaphore_init(&rsem, 1);
    semaphore_init(&wsem, 1);


    for (int i = 0; i < writer_number; ++i) {

        pthread_create(&writer_thread[i], NULL, writer, (void *) writer_number);
    }

    for (int i = 0; i < reader_number; ++i) {

        pthread_create(&reader_thread[i], NULL, reader, (void *) writer_number);
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


    err = pthread_mutex_destroy(&z);
    if (err != 0) {
        perror("Failed to destroy reader writer mutex");
        exit(EXIT_FAILURE);
    }
    err = pthread_mutex_destroy(&mutex_readcount);
    if (err != 0) {
        perror("Failed to destroy reader writer mutex");
        exit(EXIT_FAILURE);
    }
    err = pthread_mutex_destroy(&mutex_writecount);
    if (err != 0) {
        perror("Failed to destroy reader writer mutex");
        exit(EXIT_FAILURE);
    }

    semaphore_destroy(&z);
    semaphore_destroy(&wsem);
    semaphore_destroy(&rsem);
}