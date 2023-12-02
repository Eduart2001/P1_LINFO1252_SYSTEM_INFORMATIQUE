#include <semaphore.h>

int main(int argc, char const *argv[]) {
    sem_t semaphore;

    sem_init(&semaphore, 0, 1);

    sem_wait(&semaphore); //lock
    //section critique
    sem_post(&semaphore); //unlock

    sem_destroy(&semaphore);
}