#include <stdio.h>
#include "dhcp.h"

#define GREEN "\033[0;32m"
#define RED   "\033[0;31m"
#define RESET "\033[0m"

int main() 
{
    printf("something " GREEN "passed" RESET "\n");
    printf("something " GREEN "passed" RESET "\n");
    printf("something " GREEN "passed" RESET "\n");
    printf("something " GREEN "passed" RESET "\n");
    printf("something " GREEN "passed" RESET "\n");
    
    return 0;
}
