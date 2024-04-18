#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main() {
    void* handle;
    int (*test_collatz_convergence)(int, int);
    char* error;

    handle = dlopen("libcollatz.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Cannot open library: %s\n", dlerror());
        exit(1);
    }

    dlerror();

    test_collatz_convergence = (int (*)(int, int)) dlsym(handle, "test_collatz_convergence");
    if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "Cannot find symbol: %s\n", error);
        exit(1);
    }

    int number = 27;
    int result = test_collatz_convergence(number, 1000);
    printf("Test Collatz Convergence for %d: %d iterations\n", number, result);

    dlclose(handle);
}

