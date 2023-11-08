/*
 * File: sig_example1.c
 * Author: Susan Fox
 * Fall 2023
 * Contains a demo showing how a program can handle (catch and respond to) various
 * signals from the OS
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

/* signal handler for SIGALRM */
void sigalarm_handler(int sig) {
    printf("\nBEEP, signal number %d\n.", sig);
    fflush(stdout);
    alarm(5);  /* sends another SIGALRM in 5 seconds */
}

/* signal handler for SIGCONT */
void sigcont_handler(int sig) {
    printf("\nIn SIGCONT handler function, signal number %d\n.", sig);
    fflush(stdout);
}

/* signal handler for SIGINT */
void sigint_handler(int sig) {
    printf("\nIn SIGINT handler function, signal number %d...exiting\n.", sig);
    fflush(stdout);
    exit(0);
}

/* main: register signal handlers and repeatedly block until receive signal */
int main(void) {

    /* Register signal handlers. */
    if (signal(SIGCONT, sigcont_handler) == SIG_ERR) {
        printf("Error call to signal, SIGCONT\n");
        exit(1);
    }

    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        printf("Error call to signal, SIGINT\n");
        exit(1);
    }

    if (signal(SIGALRM, sigalarm_handler) == SIG_ERR) {
        printf("Error call to signal, SIGALRM\n");
        exit(1);
    }

    alarm(5);  /* sends a SIGALRM in 5 seconds */

    while(1) {
        printf("kill -CONT %d to send SIGCONT\n", getpid());
        printf("kill -INT %d to send SIGINT\n", getpid());
        pause(); /* wait for a signal to happen */
    }
}
