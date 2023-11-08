/*
 * File: exec2.c
 * Author: Susan Fox
 * Fall 2023
 * Contains a demo to show one process forking a second, and the second forking
 * a third, and then both waiting for the child process to end.
 */

#include <stdio.h>   // for printf
#include <stdlib.h>  // for rand and
#include <unistd.h>  // for fork and sleep
#include <time.h>    // for time
#include <sys/types.h>  // for pid_t
#include <sys/wait.h>  // for wait


int main(void) {
    pid_t pid1, pid2, ret;
    int status;

    printf("A\n");

    pid1 = fork();
    if (pid1 == 0) { /* child 1 */
        printf("B\n");
        pid2 = fork();
        if (pid2 == 0) {   /* child 2 */
            printf("C\n");
        }
        else {             /* child 1 (parent of child 2) */
            ret = wait(&status);
            printf("D\n");
            exit(0);
        }
    }
    else {                  /* original parent */
        printf("E\n");
        ret = wait(&status);
        printf("F\n");
    }
    return 0;
}   
