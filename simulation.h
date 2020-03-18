#ifndef SIMULATION_H
#define SIMULATION_H

#include "LIST.h"

enum state {RUNNING, READY, BLOCKED, EXITED };

typedef struct PCB {
    int pid;
    int priority;
    enum state procState;
    char procMsg[50];
} PCB;

typedef struct SEMAPHORE{
    int value;
    LIST* procList;
} SEMAPHORE;


//init the simulation,create the init process
//keep read the command and handle
void initSimulation();

//create the process with priority
int createProcess(int priority);

//fork the process,if the process is scheduler,just return
int forkProcess();

//kill the process with the pid
int killProcess(int pid);

//Kill the current running process.
int exitProcess();

//Time quantum of running process expires
int quantum();

//Send a message to another process - block until reply
int send(int pid,char* msg);

// receive a message - block until one arrives.
int receive();

// unblocks sender and delivers reply
int reply(int pid,char* msg);

//Initialize the named semaphore with the value given.
int newSemaphore(int id,int value);

//execute the semaphore P operation.
int semaphoreP(int id);

//execute the semaphore V operation.
int semaphoreV(int id);

//Print the process's info.
void processInfo(int pid);

// Display all process queues and their contents.
void totalInfo();

#endif
