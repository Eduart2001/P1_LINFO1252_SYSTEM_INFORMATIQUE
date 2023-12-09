#include "./test-and-set.h"

typedef struct queue_node {
    pthread_t thread_id;
    struct queue_node* next;
} queue_node;

typedef struct semaphore {
    volatile int val;
    my_mutex *mutex;
    queue_node* front;
    queue_node* rear;
} my_semaphore;

int my_sem_init(my_semaphore *s, unsigned int value);

int my_sem_destroy(my_semaphore *s);

int my_sem_wait(my_semaphore *s);

int my_sem_post(my_semaphore *s);