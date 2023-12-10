#include "../headers/my_semaphore.h"

void enqueue(my_semaphore *s, pthread_t thread_id) {
    queue_node* new_node = (queue_node*)malloc(sizeof(queue_node));
    new_node->thread_id = thread_id;
    new_node->next = NULL;
    if (s->rear == NULL) {
        s->front = new_node;
        s->rear = new_node;
    } else {
        s->rear->next = new_node;
        s->rear = new_node;
    }
}

pthread_t dequeue(my_semaphore *s) {
    if (s->front == NULL) {
        return (pthread_t)NULL; // Cola vacía
    }
    queue_node* temp = s->front;
    s->front = s->front->next;
    if (s->front == NULL) {
        s->rear = NULL;
    }
    pthread_t thread_id = temp->thread_id;
    free(temp);
    return thread_id;
}

void my_sem_init(my_semaphore *s, unsigned int value) {
    s->val = value;
    s->front = NULL;
    s->rear = NULL;
    my_mutex *mut = (my_mutex *)malloc(sizeof(my_mutex));
    s->mutex = mut;
    my_mutex_init(s->mutex);
    return 0;
}

void my_sem_destroy(my_semaphore *s) {
    my_mutex_destroy(s->mutex);
    while (s->front != NULL) {
        queue_node* temp = s->front;
        s->front = s->front->next;
        free(temp);
    }
    free(s);
    return 0;
}

void my_sem_wait(my_semaphore *s) {
    //test_and_test_and_set_lock(s->mutex);
    while (s->val <= 0) {}
    s->val--;
    my_mutex_unlock(s->mutex);
}

void my_sem_post(my_semaphore *s) {
    s->val++;
}