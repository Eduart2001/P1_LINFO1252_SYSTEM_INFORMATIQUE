#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

typedef struct mutex{
    volatile int *lock;
} my_mutex;

typedef struct threads_args {
    my_mutex *mutex;
    int n_cycles;
} my_threads_args;


int my_mutex_init(my_mutex *mutex);

int my_mutex_destroy(my_mutex *mutex);

int my_mutex_lock(my_mutex *mutex);

int my_mutex_unlock(my_mutex *mutex);