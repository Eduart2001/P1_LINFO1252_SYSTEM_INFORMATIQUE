#include <stdio.h>
#include <stdbool.h>

#define A 0
#define B 1
int flag[];
int turn;

void thread_A() {
    flag[A]=true;
    turn=B;
    while((flag[B]==true)&&(turn==B)) { /* loop */ }
    section_critique();
    flag[A]=false;
}

void thread_B() {
    flag[B]=true;
    turn=A;
    while((flag[A]==true)&&(turn==A)) { /* loop */ }
    section_critique();
    flag[B]=false;
}

int main(int argc, char const *argv[]) {
    flag[A]=false;
    flag[B]=false;
    return 0;
}