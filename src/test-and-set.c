#include "../headers/test-and-set.h"

int init(my_mutex *mutex) {
    if (mutex == NULL) {
        printf("Please give a pointer to a valid mutex structure");
        return 1;
    }
    mutex->lock = (int *)malloc(sizeof(int));
    if (mutex->lock == NULL) {
        printf("Error mutex lock variable malloc");
        return 2;
    }
    asm volatile("movl $0, %0\n" :"=m"(*(mutex->lock)));  // ; initialise lock à 0
    return 0;
}

int my_mutex_lock(my_mutex *mutex) {
    int temp;
    do {
        temp = 1;
        asm volatile("xchgl %0, %1\n": "+r"(temp): "m"(*(mutex->lock)): "memory"); // On boucle sur la variable temp qui prend la valeur du registre
    } while (temp != 0);
    return 0;
}

int my_mutex_unlock(my_mutex *mutex) {
    asm volatile("movl $0, %%eax\n"
                 "xchg %%eax, %0\n": "+r"(*(mutex->lock)): "m"(*(mutex->lock)): "memory");
    return 0;
}

void test(int n_threads) {
    
}

// *Uncomment to test*

// int main(int argc, char const *argv[]) {
//     my_mutex *mutex = (my_mutex *)malloc(sizeof(my_mutex));
//     init(mutex);
//     printf("%d\n", *mutex->lock);


    
//     //section critique
//     printf("%d\n", my_mutex_lock(mutex));
//     printf("SC: %d\n", *mutex->lock);
//     // my_mutex_lock(mutex); //ça bloque le mutex
//     //printf("ok\n");
//     // my_mutex_unlock(mutex);
//     // printf("%d\n", *mutex->lock);

//     // int x = 10, y;
  
//     // asm ("movl %1, %%eax;"
//     //     "movl %%eax, %0;"
//     //     :"=r"(y)  /* y is output operand */
//     //     :"r"(x)   /* x is input operand */
//     //     :"%eax"); /* %eax is clobbered register */
//     // printf("%d\n", y);
//     return 0;
// }