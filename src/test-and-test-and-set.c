#include "../headers/test-and-set.h"

int test_and_test_and_set_lock(my_mutex *mutex) { //test-and test-and-set
    int temp;
    do {
        while (*(mutex->lock) == 1) {}
        temp = 1;
        asm volatile("xchgl %0, %1\n": "+r"(temp): "m"(*(mutex->lock)): "memory"); // On boucle sur la variable temp qui prend la valeur du registre
    } while (temp != 0);
    return 0;
}

void *thread_func(void *arg){
    my_threads_args argument = *((my_threads_args *) arg);
    for (int i = 0; i < argument.n_cycles; i++) {
        test_and_test_and_set_lock(argument.mutex);
        //section critique
        for (int j=0; j<10000; j++);
        my_mutex_unlock(argument.mutex);
    }
    return (NULL);
}

void test(int n_threads) {
    pthread_t threads[n_threads];
    my_threads_args **args = (my_threads_args **)malloc(n_threads*sizeof(my_threads_args));
    my_mutex *test_mutex = (my_mutex *)malloc(sizeof(my_mutex));
    my_mutex_init(test_mutex);
    for (int i = 0; i < n_threads-1; i++) {
        args[i] = (my_threads_args *)malloc(sizeof(my_threads_args));
        args[i]->mutex = test_mutex;
        (args[i])->n_cycles = 6400/n_threads;
        pthread_create(&(threads[i]), NULL, &thread_func, args[i]);
    }
    args[n_threads-1] = (my_threads_args *)malloc(sizeof(my_threads_args));
    args[n_threads-1]->mutex = test_mutex;
    args[n_threads-1]->n_cycles = 6400/n_threads + (6400%n_threads);
    pthread_create(&(threads[n_threads-1]), NULL, &thread_func, args[n_threads-1]);


    for (int i = 0; i < n_threads; ++i) {
        int error = pthread_join(threads[i], NULL); // Aquí peta
        if (error != 0) {
            perror("Failed to join thread");
            exit(EXIT_FAILURE);
        }
    }

    my_mutex_destroy(test_mutex);
}

int main(int argc, char const *argv[]) {
    clock_t begin = clock();
    int number_threads = 0;
    for (int i=0; i<argc-2; i++){
        if (strcmp(argv[i+1], "-N") == 0) number_threads = atol(argv[i+2]);
    }
    if (number_threads <= 0) {
        printf("Please give number of threads after -N\n");
        return (EXIT_FAILURE);
    }
    test(number_threads);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f\n", time_spent);
    // my_mutex *mutex = (my_mutex *)malloc(sizeof(my_mutex));
    // init(mutex);
    // printf("%d\n", *mutex->lock);


    
    //section critique
    // printf("%d\n", my_mutex_lock(mutex));
    // printf("SC: %d\n", *mutex->lock);
    // my_mutex_lock(mutex); //ça bloque le mutex
    //printf("ok\n");
    // my_mutex_unlock(mutex);
    // printf("%d\n", *mutex->lock);

    // int x = 10, y;
  
    // asm ("movl %1, %%eax;"
    //     "movl %%eax, %0;"
    //     :"=r"(y)  /* y is output operand */
    //     :"r"(x)   /* x is input operand */
    //     :"%eax"); /* %eax is clobbered register */
    // printf("%d\n", y);
    return (EXIT_SUCCESS);
}