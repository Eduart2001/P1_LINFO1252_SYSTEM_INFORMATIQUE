#include "../headers/error.h"

void error(char *msg) {
    fprintf(stderr,"Error : %s\n", msg);
    exit(EXIT_FAILURE);
}