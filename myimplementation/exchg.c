#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[]) {
    asm("lock:"                     // ; étiquette, variable
        "   .long   0"              // ; initialisée à 0
        
        "enter:"
        "   movl    $1, %eax"       // ; %eax=1
        "   xchgl   %eax, (lock)"   // ; instruction atomique, échange (lock) et %eax
                                    // ; après exécution, %eax contient la donnée qui était
                                    // ; dans lock et lock la valeur 1
        "   testl   %eax, %eax"     // ; met le flag ZF à vrai si %eax contient 0
        "   jzn     enter"          // ; retour à enter: si ZF n'est pas vrai
        "   ret"
        
        "leave:"
        "   movl    $0, %eax"       // ; %eax=0
        "   xchgl   %eax, (lock)"   // ; instruction atomique
        "   ret");
    return 0;
}