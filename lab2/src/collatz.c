#include <stdio.h>

int collatz_conjecture(int input){
    if(input % 2 == 0){
        return input / 2;
    }
    else{
        return 3*input + 1;
    }
}

int test_collatz_convergence(int input, int max_iter){
    int counter = 0;
    while(input != 1){
        if(counter == max_iter){
            return -1;
        }
        input = collatz_conjecture(input);
        counter++;
    }
    return counter;
}
