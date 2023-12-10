#include "../headers/my_mutex.h"
#include "../headers/error.h"

void my_mutex_init(my_mutex *mutex) {
    if (mutex == NULL) error("Please give a pointer to a valid mutex structure");
    mutex->lock = (int *)malloc(sizeof(int));
    if (mutex->lock == NULL) error("mutex lock variable malloc failed");
    asm volatile("movl $0, %0\n" :"=m"(*(mutex->lock)));  // ; initialize lock at 0
}

void my_mutex_destroy(my_mutex *mutex) {
    if (mutex == NULL) error("Please give a pointer to a valid mutex structure");
    free((void *)mutex->lock);
    free(mutex);
}

void my_mutex_TAS_lock(my_mutex *mutex) { //test-and-set
    if (mutex == NULL) error("Please give a pointer to a valid mutex structure");
    int temp;
    do {
        temp = 1;
        asm volatile("xchgl %0, %1\n": "+r"(temp): "m"(*(mutex->lock)): "memory"); // We loop on variable temp who takes the value of the register
    } while (temp != 0);
}

void my_mutex_TATAS_lock(my_mutex *mutex) { //test-and test-and-set
    if (mutex == NULL) error("Please give a pointer to a valid mutex structure");
    int temp;
    do {
        while (*(mutex->lock) == 1) {}
        temp = 1;
        asm volatile("xchgl %0, %1\n": "+r"(temp): "m"(*(mutex->lock)): "memory"); // We loop on variable temp who takes the value of the register
    } while (temp != 0);
}

void my_mutex_unlock(my_mutex *mutex) {
    if (mutex == NULL) error("Please give a pointer to a valid mutex structure");
    asm volatile("movl $0, %%eax\n"
                 "xchg %%eax, %0\n": "+r"(*(mutex->lock)): "m"(*(mutex->lock)): "memory");
}