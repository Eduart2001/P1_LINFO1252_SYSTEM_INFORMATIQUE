#ifndef LINFO1252__MY_MUTEX__
#define LINFO1252__MY_MUTEX__

typedef struct my_mutex{
    volatile int *lock;
} my_mutex;

typedef struct my_threads_args {
    my_mutex *mutex;
    int n_cycles;
} my_threads_args;


void my_mutex_init(my_mutex *mutex);

void my_mutex_destroy(my_mutex *mutex);

void my_mutex_TAS_lock(my_mutex *mutex);

void my_mutex_TATAS_lock(my_mutex *mutex);

void my_mutex_unlock(my_mutex *mutex);

#endif // LINFO1252__MY_MUTEX__