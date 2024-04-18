#include "collatz.h"
#include <stdio.h>

int main(){
    int number = 27;
    int result = test_collatz_convergence(number, 1000);
    printf("Test Collatz Convergence for %d: %d iterations\n", number, result);
    return 0;
}
