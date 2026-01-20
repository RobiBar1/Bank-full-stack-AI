#include <stdio.h>
#include "task.h"

#define GREEN "\033[0;32m"
#define RED   "\033[0;31m"
#define RESET "\033[0m"

int main() {
    printf("something " GREEN "passed" RESET "\n");
    printf("something " RED "failed" RESET "\n");
    return 0;
}
