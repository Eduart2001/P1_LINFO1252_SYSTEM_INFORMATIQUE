#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

sem_t rendezvous;
pthread_mutex_t mutex;
int count=0;
sem_init(&rendezvous,0,0);

// section critique
pthread_mutex_lock(&mutex);
count++;
if(count==N) {
// tous les threads sont arrivés
sem_post(&rendezvous);
}
pthread_mutex_unlock(&mutex);
// attente à la barrière
sem_wait(&rendezvous);
// libération d’un autre thread en attente
sem_post(&rendezvous);
seconde_phase();


// Le pseudo-code ci-dessus présente une solution permettant de résoudre ce problème du rendez-vous. Le
// sémaphore étant initialisé à 0, le premier thread qui aura terminé la première phase sera bloqué sur
// 130 Chapitre 6. Systèmes Multiprocesseurs
// Syllabus LEPL1503 et LINFO1252, Version 2020
// sem_wait(&rendezvous);. Les N-1 premiers threads qui auront terminé leur première phase seront tous
// bloqués à cet endroit. Lorsque le dernier thread finira sa première phase, il incrémentera count puis exécutera
// sem_post(&rendezvous); ce qui libèrera un premier thread. Le dernier thread sera ensuite bloqué sur
// sem_wait(&rendezvous); mais il ne restera pas bloqué longtemps car chaque fois qu’un thread parvient
// à passer sem_wait(&rendezvous);, il exécute immédiatement sem_post(&rendezvous); ce qui permet de libérer un autre thread en cascade