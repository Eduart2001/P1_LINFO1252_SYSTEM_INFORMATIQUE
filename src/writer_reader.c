#include "../headers/error.h"

#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define WRITER_CYCLES 640
#define READER_CYCLES 2560
/* Initialization */
pthread_mutex_t mutex_readcount; // protects readcount
pthread_mutex_t mutex_writecount; // protects writecount
pthread_mutex_t z; // Only one reader waiting

sem_t wsem; // exclusive access to the db
sem_t rsem; // To block the readers

int readcount=0;
int writecount=0;

int verbose = false;

// for testing purpose
int total_reads = 0;
int total_writes = 0;

void  *writer (void *arg){
    if (verbose) printf("writer cycles inside: %d\n", *(int *)arg); // for testing purpose
    for(int i=0; i<*((int*)arg); i++) {
        if(pthread_mutex_lock(&mutex_writecount)) error("initial writer mutex_lock failed");
        
        // for testing purpose
        if(verbose) {
            total_writes++;
            printf("total writes: %d\n", total_writes);
        }
        
        // critical section - writecount
        writecount++;
        if(writecount==1) if(sem_wait(&rsem)) error("first writer sem_wait failed"); // First writer arriving
        if(pthread_mutex_unlock(&mutex_writecount)) error("writer first mutex_unlock failed");
        if(sem_wait(&wsem)) error("writer sem_wait failed"); // critical section, only one writer at a time
        
        for (int i=0; i<10000; i++); // hard work
        if(sem_post(&wsem)) error("writer sem_post failed");
        if(pthread_mutex_lock(&mutex_writecount)) error("writer second mutex_lock failed");
        // section critique - writecount
        writecount--;
        if(writecount==0) {
            if(sem_post(&rsem)) error("writer last sem_post failed"); // departure of the last writer
        }
        if(pthread_mutex_unlock(&mutex_writecount)) error("writer last mutex_unlock failed");
    }
    pthread_exit(NULL);
}

void *reader(void *arg){
    if(verbose) printf("reader cycles inside: %d\n", *(int *)arg); // for testing purpose
    for(int i=0; i<*((int*)arg); i++) {
        if(pthread_mutex_lock(&z)) error("initial reader z mutex_lock failed");
        // mutual exclusion, only one reader waiting on rsem
        if(sem_wait(&rsem)) error("reader sem_wait failed");
        if(pthread_mutex_lock(&mutex_readcount)) error("reader first readcount mutex_lock failed");
        
        // for testing purpose
        if(verbose) {
            total_reads++;
            printf("total reads: %d\n", total_reads);
        }
        
        // mutual exclusion, readercount
        readcount++;
        if (readcount==1) {
            if(sem_wait(&wsem)) error("reader sem_wait failed"); // First reader arriving
        }

        if(pthread_mutex_unlock(&mutex_readcount)) error("reader mutex_unlock failed");
        if(sem_post(&rsem)) error("reader sem_post faield"); // release next reader/writer
        
        if(pthread_mutex_unlock(&z)) error("reader z mutex_unlock failed");
        for (int i=0; i<10000; i++); // hard work
        
        if(pthread_mutex_lock(&mutex_readcount)) error("reader second readcount mutex_lock failed");
        // mutual exclusion, readcount
        readcount--;
        if(readcount==0) {
            if(sem_post(&wsem)) error("reader last sem_post failed"); // departure of the last reader
        }
        if(pthread_mutex_unlock(&mutex_readcount)) error("reader last mutex_unlock failed");
    }
    pthread_exit(NULL);
}

int main(int argc, char const *argv[]) {
    clock_t begin = clock();
    int reader_number = 0;
    int writer_number = 0;

    for (int i=0; i<argc-2; i++){
        if (strcmp(argv[i+1], "-R") == 0) reader_number = atoi(argv[i+2]);
        else if (strcmp(argv[i+1], "-W") == 0) writer_number = atoi(argv[i+2]);
        else if (strcmp(argv[i+1], "-v") == 0) verbose = true;
    }
    if (strcmp(argv[argc-1], "-v") == 0) verbose = true;

    if (reader_number <= 0 || writer_number <= 0) error("Please give a valid writer or consumer number of threads");

    if(pthread_mutex_init(&mutex_readcount, NULL)) error("pthread_mutex_init readcount");

    if(pthread_mutex_init(&mutex_writecount, NULL)) error("pthread_mutex_init writecount");

    if(pthread_mutex_init(&z, NULL)) error("pthread_mutex_init only one reader (z)");

    if(sem_init(&wsem, 0 , 1)) error("sem_init_empty wsem"); // empty buffer

    if (sem_init(&rsem, 0 , 1)) error("sem_init_full rsem"); // empty buffer

    pthread_t reader_thread[reader_number];
    pthread_t writer_thread[writer_number];

    int cycles_writer[writer_number];

    for (int i = 0; i < writer_number; i++) {
        if (i == writer_number-1) {
            cycles_writer[i] = (WRITER_CYCLES/writer_number) + (WRITER_CYCLES%writer_number);
            //printf("cycles_writer outside: %d\n", cycles_writer); // for testing purpose
            if (pthread_create(&(writer_thread[i]), NULL, &writer, (void *) &(cycles_writer[i]))) error("writer pthread_create failed");
        }
        else {
            cycles_writer[i] = WRITER_CYCLES/writer_number;
            //printf("cycles_writer outside: %d\n", cycles_writer); // for testing purpose
            if(pthread_create(&(writer_thread[i]), NULL, &writer, (void *) &(cycles_writer[i]))) error("last writer pthread_create failed");
        }
    }

    int cycles_reader[reader_number];

    for (int i = 0; i < reader_number; i++) {
        if (i == reader_number-1){
            cycles_reader[i] = (READER_CYCLES/reader_number) + (READER_CYCLES%reader_number);
            //printf("cycles_reader outside: %d\n", cycles_reader); // for testing purpose
            if(pthread_create(&(reader_thread[i]), NULL, &reader, (void *) &(cycles_reader[i]))) error("reader pthread_create failed");
        }
        else {
            cycles_reader[i] = READER_CYCLES/reader_number;
            //printf("cycles_reader outside: %d\n", cycles_reader); // for testing purpose
            if(pthread_create(&(reader_thread[i]), NULL, &reader, (void *) &(cycles_reader[i]))) error("last reader pthread_create failed");
        }
    }

    // writer and reader: join and clean up
    for (int i = 0; i < writer_number; ++i) {
        if(pthread_join(writer_thread[i], NULL)) error("Failed to join writer thread");
        if(pthread_join(reader_thread[i], NULL)) error("Failed to join reader thread");
    }

    pthread_mutex_destroy(&mutex_readcount);
    pthread_mutex_destroy(&mutex_writecount);
    pthread_mutex_destroy(&z);
    sem_destroy(&wsem);
    sem_destroy(&rsem);
    if (verbose) {
        printf("total reads: %d, total writes: %d\n", total_reads, total_writes);
        printf("READER_CYCLES: %d, WRITER_CYCLES: %d\n", READER_CYCLES, WRITER_CYCLES);
    }

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f\n", time_spent);
    
    return (EXIT_SUCCESS);
}