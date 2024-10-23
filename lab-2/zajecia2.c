#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    pid_t pid = getpid(); // id procesu
    printf("Imię i nazwisko: Anna Jasielec, numer procesu: %d\n", pid);
    printf("%s", argv[0]);
    return 0;
}