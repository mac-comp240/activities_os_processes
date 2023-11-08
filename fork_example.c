/*
 * File: fork_example.c
 * Author: Susan Fox
 * Fall 2023
 * Contains a demo program to show how fork works. Each process sleeps for a random
 * time between 1 and 3 seconds.
 */

#include <stdio.h>   // for printf
#include <stdlib.h>  // for rand and
#include <unistd.h>  // for fork and sleep
#include <time.h>    // for time
#include <sys/types.h>  // for pid_t?


int main(void) {
    pid_t pid, mypid;
    int rVal;

    printf("A\n");

    pid = fork();   /* create a new child process */

    if(pid == -1) {  /* check and handle error return value */
        printf("fork failed!\n");
        exit(pid);
    }

    if (pid == 0) { /* the child process */
        mypid = getpid();
        printf("            Child: fork returned %d, my pid %d\n", pid, mypid);

    } else  {  /* the parent process */
        mypid = getpid();
        printf("Parent: fork returned %d, my pid %d\n", pid, mypid);
    }

    printf("B:%d\n", mypid);

    return 0;
}

