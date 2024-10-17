#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid = getpid(); // id procesu
    printf("Imię i nazwisko: Anna Jasielec, numer procesu: %d\n", pid);
    return 0;
}