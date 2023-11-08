/*
 * File: exec_helper.c
 * Author: Susan Fox
 * Fall 2023
 * This file just prints the numbers from 1 to 100, sleeping 1 second in between.
 */

#include <stdio.h>  // for printf
#include <unistd.h>  // for sleep, getpid
#include <sys/types.h>  // for pid_t

int main(int argc, char *argv) {
    int i;
    pid_t pid = getpid();
    for (i = 0; i < 100; i++) {
        printf("      %d exec_helper: %d\n", pid, i);
        sleep(1);
    }
    return 0;
}