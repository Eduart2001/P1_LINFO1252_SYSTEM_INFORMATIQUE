#include "../headers/my_semaphore.h"
#include "../headers/error.h"

#include <time.h>
#include <stdbool.h>

void thread_func() {
    
}

void test_sem() {

}

int main(int argc, char const *argv[]) {
    clock_t begin = clock();


    
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f\n", time_spent);

    return (EXIT_SUCCESS);
}