#include <stdio.h>

#include "simple_shell.h"

#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define RESET "\033[0m"

int main() { return REPLShell(0); }
