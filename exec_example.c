/*
 * File: exec_example.c
 * Author: Susan Fox
 * Fall 2023
 * Contains a demo program to show how exec works. Child process prints numbers
 * one per second, parent process just prints a final message.
 */

#include <stdio.h>   // for printf
#include <stdlib.h>  // for rand and
#include <unistd.h>  // for fork and sleep
#include <time.h>    // for time
#include <sys/types.h>  // for pid_t
#include <sys/wait.h>  // for wait

void wait_for_child();


int main(void) {
    pid_t pid;
    int  ret;
    int status;
    char *argv[2];

    // Set up the executable we want the child to run
    argv[0] = "exec_helper";
    argv[1] = NULL;

    pid = fork();
    if (pid == 0) { /* child process */
        ret = execv("exec_helper", argv);   // child runs exec_helper executable
        if (ret < 0) {
            printf("Error: execvp returned!!!\n");
            exit(ret);
        }
    }
    else {  /* parent process */
        printf("Parent process done now\n");
    }
    return 0;
}   


void wait_for_child() {
    int  ret;
    int status;
    pid_t pid;

    printf("Parent process waiting on child process to end...\n");
    pid = wait(&status);
    printf("Child process %d ended\n", pid);
    if (WIFEXITED(status)) {
        printf("Child ended with exit(%d)\n", WEXITSTATUS(status));
    }
    if (WIFSIGNALED(status)) {
        printf("Child process ended with kill -%d\n", WTERMSIG(status));
    }
}

 