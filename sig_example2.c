/*
 * File: sig_example2.c
 * Author: Susan Fox
 * Fall 2023
 * Contains a demo showing how to handle SIGCHLD signals from child processes
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>


/*
 * signal handler for SIGCHLD: reaps zombie children
 *  signum: the number of the signal (will be 20 for SIGCHLD)
 */
void sigchld_handler(int signum) {
    int status;
    pid_t pid;

    /*
     * reap any and all exited child processes
     * (loop because there could be more than one)
     */
    while( (pid = waitpid(-1, &status, WNOHANG)) > 0) {
        // uncomment debug print stmt to see what is being handled
        printf("SIGCHLD signal received %d me:%d child: %d\n", signum, getpid(), pid);
    }
}

int main(void) {
    pid_t pid;
    int ret;
    int cnt;
    char *argv[2];

    // Set up the executable we want the child to run
    argv[0] = "exec_helper";
    argv[1] = NULL;

    /* register SIGCHLD handler: */
    if ( signal(SIGCHLD, sigchld_handler) == SIG_ERR) {
        printf("ERROR signal failed\n");
	exit(1);
    }

    /* create a child process */
    pid = fork();
    if(pid == 0) {         /* child process */
        ret = execv("exec_helper", argv);   // child runs exec_helper executable
        if (ret < 0) {
            printf("Error: execvp returned!!!\n");
            exit(ret);
        }
    }
    else {     /* parent process executes concurrently with child */
        pid_t  mypid = getpid();
        int j;
        for (j=100; j > 0; j--) {
            printf("%d Parent %d\n", mypid, j);
            sleep(1.0);
        }
    }

    return 0;
}