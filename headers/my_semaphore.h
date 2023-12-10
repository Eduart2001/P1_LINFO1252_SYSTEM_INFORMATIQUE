#ifndef LINFO1252__MY_SEMAPHORE__
#define LINFO1252__MY_SEMAPHORE__

#include "./my_mutex.h"
#include <pthread.h>

typedef struct semaphore {
    volatile int val;
    my_mutex *mutex;
} my_semaphore;

void my_sem_init(my_semaphore *s, unsigned int value);

void my_sem_destroy(my_semaphore *s);

void my_sem_wait(my_semaphore *s);

void my_sem_post(my_semaphore *s);

#endif // LINFO1252__MY_SEMAPHORE__