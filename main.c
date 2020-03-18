#include <stdio.h>
#include <stdlib.h>

#include "simulation.h"

//welcome information
void helper();

int main(int argc,char** argv)
{
    helper();
    initSimulation();
}

//welcome information
void helper()
{
    printf("(C)reate: Create the new process with pid.\n");
    printf("(F)ork: Copy the currently running process.\n");
    printf("(K)ill: kill the named process.\n");
    printf("(E)xit: Kill the current running process.\n");
    printf("(Q)uantum: Time quantum of running process expires.\n");
    printf("(S)end: Send a message to another process - block until reply.\n");
    printf("(R)eceive: receive a message - block until one arrives.\n");
    printf("Repl(Y): unblocks sender and delivers reply.\n");
    printf("(N)ew Semaphore: Initialize the named semaphore with the value given.\n");
    printf("Semaphore(P): execute the semaphore P operation.\n");
    printf("Semaphore(V): execute the semaphore V operation.\n");
    printf("Proc(I)nfo: Print the process's info.\n");
    printf("(T)otalInfo: Display all process queues and their contents.\n");
}
