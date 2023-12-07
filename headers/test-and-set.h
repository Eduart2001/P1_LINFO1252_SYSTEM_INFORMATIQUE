#include <stdio.h>
#include <stdlib.h>

typedef struct mutex{
    volatile int *lock;
} my_mutex;

int init(my_mutex *mutex);

int my_mutex_lock(my_mutex *mutex);

int my_mutex_unlock(my_mutex *mutex);