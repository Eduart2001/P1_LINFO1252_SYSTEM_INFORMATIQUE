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

/* Writer */
void  *writer (void *arg){
    while(true)
    {
        think_up_data();
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

    while(true)
    {
        pthread_mutex_lock(&z);
        // exclusion mutuelle, un seul reader en attente sur rsem
        sem_wait(&rsem);
        pthread_mutex_lock(&mutex_readcount);
        // exclusion mutuelle, readercount
        readcount=readcount+1;
        if (readcount==1) {
            // arrivée du premier reader
            sem_wait(&wsem);
        }

        pthread_mutex_unlock(&mutex_readcount);
        sem_post(&rsem); // libération prochain reader/writer
        
        pthread_mutex_unlock(&z);
        read_database();
        
        pthread_mutex_lock(&mutex_readcount);
        // exclusion mutuelle, readcount
        readcount=readcount-1;
        if(readcount==0) {
            // départ du dernier reader
            sem_post(&wsem);
        }
        pthread_mutex_unlock(&mutex_readcount);
        use_data_read();
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

    err = pthread_mutex_init(&mutex_writecount, NULL);
    if(err!=0) error(err,"pthread_mutex_init only one reader");

    err = sem_init(&wsem, 0 , 1); // buffer vide
    if(err!=0) error(err,"sem_init_empty wsem");

    err = sem_init(&rsem, 0 , 1); // buffer vide
    if(err!=0) error(err,"sem_init_full rsem");




    pthread_t reader_thread[reader_number];
    pthread_t writer_thread[writer_number];

    int i;
    for(i=0; i<reader_number; i++) {
        pthread_create(&reader_thread[i], NULL, reader, NULL);
    }

    for(i=0; i<writer_number; i++) {
        pthread_create(&writer_thread[i], NULL, writer, NULL);
    }

    for(i=0; i<reader_number; i++) {
        pthread_join(reader_thread[i], NULL);
    }

    for(i=0; i<writer_number; i++) {
        pthread_join(writer_thread[i], NULL);
    }

    pthread_mutex_destroy(&mutex_readcount);
    pthread_mutex_destroy(&mutex_writecount);
    pthread_mutex_destroy(&z);
    sem_destroy(&wsem);
    sem_destroy(&rsem);

    return 0;
}