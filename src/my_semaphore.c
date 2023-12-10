#include "../headers/my_semaphore.h"
#include "../headers/error.h"

void my_sem_init(my_semaphore *s, unsigned int value) {
    s->val = value;
    my_mutex *mut = (my_mutex *)malloc(sizeof(my_mutex));
    if(mut == NULL) error("my_sem_init mutex malloc failed");
    my_mutex_init(mut);
    s->mutex = mut;
}

void my_sem_destroy(my_semaphore *s) {
    my_mutex_destroy(s->mutex);
    free(s);
}

void my_sem_wait(my_semaphore *s) {
    my_mutex_TATAS_lock(s->mutex);
    while (s->val <= 0) {}
    (s->val)--;
    my_mutex_unlock(s->mutex);
}

void my_sem_post(my_semaphore *s) {
    (s->val)++;
}