#include "../headers/my_mutex.h"
#include "../headers/error.h"

#include <pthread.h>
#include <time.h>
#include <stdbool.h>

void *thread_TAS_func(void *arg){
    if (arg == NULL) error("Please give a pointer to a valid mutex structure");
    my_threads_args argument = *((my_threads_args *) arg);
    for (int i = 0; i < argument.n_cycles; i++) {
        my_mutex_TAS_lock(argument.mutex);
        // critical section
        for (int j=0; j<10000; j++);
        my_mutex_unlock(argument.mutex);
    }
    return (NULL);
}

void *thread_TATAS_func(void *arg){
    if (arg == NULL) error("Please give a pointer to a valid mutex structure");
    my_threads_args argument = *((my_threads_args *) arg);
    for (int i = 0; i < argument.n_cycles; i++) {
        my_mutex_TATAS_lock(argument.mutex);
        // critical section
        for (int j=0; j<10000; j++);
        my_mutex_unlock(argument.mutex);
    }
    return (NULL);
}

void test_TAS(int n_threads) {
    pthread_t threads[n_threads];
    my_threads_args **args = (my_threads_args **)malloc(n_threads*sizeof(my_threads_args));
    my_mutex *test_mutex = (my_mutex *)malloc(sizeof(my_mutex));
    my_mutex_init(test_mutex);
    for (int i = 0; i < n_threads-1; i++) {
        args[i] = (my_threads_args *)malloc(sizeof(my_threads_args));
        args[i]->mutex = test_mutex;
        (args[i])->n_cycles = 6400/n_threads;
        if(pthread_create(&(threads[i]), NULL, &thread_TAS_func, args[i])) error("pthread_create in test_TAS failed");
    }
    args[n_threads-1] = (my_threads_args *)malloc(sizeof(my_threads_args));
    args[n_threads-1]->mutex = test_mutex;
    args[n_threads-1]->n_cycles = 6400/n_threads + (6400%n_threads);
    if(pthread_create(&(threads[n_threads-1]), NULL, &thread_TAS_func, args[n_threads-1])) error("last pthread_create in test_TAS failed");


    for (int i = 0; i < n_threads; ++i) {
        if(pthread_join(threads[i], NULL)) error("failed to join TAS thread"); // Aquí peta
    }

    my_mutex_destroy(test_mutex);
}

void test_TATAS(int n_threads) {
    pthread_t threads[n_threads];
    my_threads_args **args = (my_threads_args **)malloc(n_threads*sizeof(my_threads_args));
    my_mutex *test_mutex = (my_mutex *)malloc(sizeof(my_mutex));
    my_mutex_init(test_mutex);
    for (int i = 0; i < n_threads-1; i++) {
        args[i] = (my_threads_args *)malloc(sizeof(my_threads_args));
        args[i]->mutex = test_mutex;
        (args[i])->n_cycles = 6400/n_threads;
        if(pthread_create(&(threads[i]), NULL, &thread_TATAS_func, args[i])) error("pthread_create in test_TATAS failed");
    }
    args[n_threads-1] = (my_threads_args *)malloc(sizeof(my_threads_args));
    args[n_threads-1]->mutex = test_mutex;
    args[n_threads-1]->n_cycles = 6400/n_threads + (6400%n_threads);
    if(pthread_create(&(threads[n_threads-1]), NULL, &thread_TATAS_func, args[n_threads-1])) error("last pthread_create in test_TATAS failed");


    for (int i = 0; i < n_threads; ++i) {
        if(pthread_join(threads[i], NULL)) error("failed to join TATAS thread"); // Aquí peta
    }

    my_mutex_destroy(test_mutex);
}

// *Uncomment to test*

int main(int argc, char const *argv[]) {
    clock_t begin = clock();
    int TAS = false;
    int TATAS = false;
    int number_threads = 0;
    for (int i=0; i<argc-2; i++){
        if (strcmp(argv[i+1], "-N") == 0) number_threads = atol(argv[i+2]);
        else if (strcmp(argv[i+1], "-TAS") == 0) TAS = true;
        else if (strcmp(argv[i+1], "-TATAS") == 0) TATAS = true;
    }
    if (strcmp(argv[argc-1], "-TAS") == 0) TAS = true;
    else if(strcmp(argv[argc-1], "-TATAS") == 0) TATAS = true;

    if ((TAS == false && TATAS == false) || (TAS == true && TATAS == true)) error("Please provide either -TAS xor -TATAS to run respective tests");
    if (number_threads <= 0) error("Please provide a valid number of threads after -N");
    if(TAS == true) test_TAS(number_threads);
    else test_TATAS(number_threads);
    
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f\n", time_spent);

    return (EXIT_SUCCESS);
}