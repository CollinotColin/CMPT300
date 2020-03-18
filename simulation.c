#include "simulation.h"

#include "Memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

PCB* pcbList[400];//the process list
LIST* readyQueue[3];//the ready queue with 0,1,2 priority
LIST* sendQueue;//send wait queue
LIST* receiveQueue;//receive wait queue
SEMAPHORE* semaphores[5];
int procCount = 0;
int* currentPid = NULL;

//print the queue
void showQueue(LIST* queue)
{
    int count = ListCount(queue);
    ListFirst(queue);
    for(int i = 0; i< count;++i)
    {
        int* n = ListCurr(queue);
        printf("%d ",*n);
        ListNext(queue);
    }
    if(count > 0 )
    {
        printf("\n");
    }
}

//for ListSearch
int isEqual(void* a,void* b)
{
    return *((int*)a) == *((int*)b);
}

//init the simulation,create the init process
//keep read the command and handle
void initSimulation()
{
    memoryInit();

    readyQueue[0] = ListCreate();
    readyQueue[1] = ListCreate();
    readyQueue[2] = ListCreate();
    sendQueue = ListCreate();
    receiveQueue = ListCreate();

    currentPid = &pcbList[createProcess(3)]->pid;

    for(int i = 0;i < 5;i++)
    {
        semaphores[i] = NULL;
    }

    int pid,semaphoreId,semaphoreValue,priority;
    while(1)
    {
        char msg[50];
        char opt = (char)getchar();
        if(opt == '\n')
        {
            continue;
        }
        switch(opt)
        {
            case 'C':
            case 'c':
                printf("enter the priority:");
                scanf("%d",&priority);
                createProcess(priority);
                break;
            case 'F':
            case 'f':
                forkProcess();
                break;
            case 'K':
            case 'k':
                printf("enter the pid:");
                scanf("%d",&pid);
                if(pid > 400)
                {
                    fprintf(stderr,"pid too large\n");
                    break;
                }
                killProcess(pid);
                break;
            case 'E':
            case 'e':
                exitProcess();
                break;
            case 'Q':
            case 'q':
                quantum();
                break;
            case 'S':
            case 's':
                printf("enter the pid:");
                scanf("%d", &pid);
                if(pid > 400)
                {
                    fprintf(stderr,"pid too large\n");
                    break;
                }
                getchar();
                printf("enter the message:");
                fgets(msg, 50, stdin);
                send(pid,msg);
                break;
            case 'R':
            case 'r':
                receive();
                break;
            case 'Y':
            case 'y':
                printf("enter the pid:");
                scanf("%d", &pid);
                if(pid > 400)
                {
                    fprintf(stderr,"pid too large\n");
                    break;
                }
                getchar();
                printf("enter the message:");
                fgets(msg, 50, stdin);
                reply(pid,msg);
                break;
            case 'N':
            case 'n':
                printf("enter the semaphore ID:");
                scanf("%d",&semaphoreId);
                printf("enter the semaphore Value:");
                scanf("%d",&semaphoreValue);
                newSemaphore(semaphoreId,semaphoreValue);
                break;
            case 'P':
            case 'p':
                printf("enter the semaphore ID:");
                scanf("%d",&semaphoreId);
                semaphoreP(semaphoreId);
                break;
            case 'V':
            case 'v':
                printf("enter the semaphore ID:");
                scanf("%d",&semaphoreId);
                semaphoreV(semaphoreId);
                break;
            case 'I':
            case 'i':
                printf("pid:");
                scanf("%d",&pid);
                if(pid > 400)
                {
                    fprintf(stderr,"pid too large\n");
                    break;
                }
                processInfo(pid);
                break;
            case 'T':
            case 't':
                totalInfo();
                break;
            default:
                fprintf(stderr,"error command\n");
                exit(1);
        }
    }
}

//create the process with priority
int createProcess(int priority)
{
    PCB* p = (PCB*)malloc(sizeof(PCB));
    p->pid = procCount;
    p->priority = priority;
    p->procState = RUNNING;
    memset(p->procMsg,0,50);
    pcbList[p->pid] = p;
    if(priority == 3)
    {//init process
        procCount+=1;
        return p->pid;
    }
    else
    {//normal process
        ListPrepend(readyQueue[priority],(void*)(&p->pid));
        procCount+=1;
        printf("process %d created\n",p->pid);
        return p->pid;
    }
}

//fork the process,if the process is scheduler,just return
int forkProcess()
{
    if(*currentPid == 0)
    {
        printf("scheduler can not be forked!\n");
        return 1;
    }
    PCB* p = (PCB*)malloc(sizeof(PCB));
    p->pid = procCount;
    p->priority = pcbList[*currentPid]->priority;
    p->procState = pcbList[*currentPid]->procState;
    strcpy(p->procMsg,pcbList[*currentPid]->procMsg);
    procCount +=1;
    pcbList[p->pid] = p;
    ListPrepend(readyQueue[p->priority],(void*)(&p->pid));
    printf("process %d forked,new process %d\n",*currentPid,p->pid);
    return 0;
}

//kill the process with the pid
int killProcess(int pid)
{
    if(pid == 0)
    {
        if(procCount == 1)
        {
            printf("scheduler exited\n");
            exit(0);
        }
        else
        {
            printf("scheduler can not be killed with another process\n");
            return 1;
        }
    }

    if(pcbList[pid]->procState == READY)
    {
        for(int i = 0; i < 3;++i)
        {
            ListFirst(readyQueue[i]);
            PCB* pcb = (PCB*)ListSearch(readyQueue[i],isEqual,&pid);
            if(pcb != NULL)
            {
                ListRemove(readyQueue[i]);
                procCount-=1;
                pcbList[pid]->procState = EXITED;
                printf("process %d killed\n",pid);
                return 0;
            }
        }
    }
    else if(pcbList[pid]->procState == BLOCKED)
    {
        pcbList[pid]->procState = EXITED;
        ListFirst(sendQueue);
        int* p = (int*)ListSearch(sendQueue,isEqual,&pid);
        if(p != NULL)
        {
            ListRemove(sendQueue);
            procCount -= 1;
            printf("process %d killed\n",pid);
            return 0;
        }
        ListFirst(receiveQueue);
        if(p != NULL)
        {
            ListRemove(receiveQueue);
            procCount -= 1;
            printf("process %d killed\n",pid);
            return 0;
        }
        for(int i = 0;i < 5;++i)
        {
            if(semaphores[i]!=NULL)
            {
                ListFirst(semaphores[i]->procList);
            }
        }
    }
    else if(pcbList[pid]->procState == RUNNING)
    {
        exitProcess();
    }
    return 1;
}

//Kill the current running process.
int exitProcess()
{
    if(*currentPid == 0)
    {
        if(procCount == 1)
        {
            printf("scheduler exited\n");
            exit(0);
        }
        else
        {
            printf("scheduler can not be killed with another process\n");
            return 1;
        }
    }

    pcbList[*currentPid]->procState = EXITED;
    printf("process exited %d\n",*currentPid);
    currentPid = NULL;
    procCount -= 1;
    quantum();
    return 0;
}

//Time quantum of running process expires
int quantum()
{
    if(currentPid != NULL)
    {
        if(*currentPid != 0)
        {//if the scheduler process,skip
            PCB* p = pcbList[*currentPid];
            if(p->priority == 0 || p->priority == 1)
            {
                p->priority += 1;
            }
            ListPrepend(readyQueue[p->priority],(void*)currentPid);
        }
        pcbList[*currentPid]->procState = READY;
        printf("process %d quantum yield\n",*currentPid);
    }
    if(ListCount(readyQueue[0]) != 0)
    {
        currentPid = (int*)ListTrim(readyQueue[0]);
    }
    else if(ListCount(readyQueue[1]) != 0)
    {
        currentPid = (int*)ListTrim(readyQueue[1]);
    }
    else if(ListCount(readyQueue[2]) != 0)
    {
        currentPid = (int*)ListTrim(readyQueue[2]);
    }
    else
    {
        currentPid = &pcbList[0]->pid;
    }

    printf("process %d quantum begin\n",*currentPid);
    PCB* pcb = pcbList[*currentPid];
    pcb->procState = RUNNING;
    if(strlen(pcb->procMsg) != 0)
    {
        printf("receiving msg: %s",pcb->procMsg);
        strcpy(pcb->procMsg,"");
    }
}

//Send a message to another process - block until reply
int send(int pid,char* msg)
{
    strcpy(pcbList[pid]->procMsg,msg);
    printf("send to process %d\n",pid);
    ListFirst(receiveQueue);
    int* p = (int*)ListSearch(receiveQueue,isEqual,&pid);
    if(p != NULL)
    {
        PCB* pcb = pcbList[pid];
        ListPrepend(readyQueue[pcb->priority],ListRemove(receiveQueue));
        printf("process %d unblocked by process %d\n",pid,*currentPid);
    }

    if(*currentPid != 0)
    {
        ListPrepend(sendQueue,(void*)currentPid);
        pcbList[*currentPid]->procState = BLOCKED;
        printf("process %d blocked with sending\n",*currentPid);
        printf("process %d quantum\n",*currentPid);
        currentPid = NULL;
        quantum();
    }
    return 0;
}

// receive a message - block until one arrives.
int receive()
{
    PCB* p = pcbList[*currentPid];
    if(strlen(p->procMsg)!=0)
    {
        printf("Receive: %s\n",p->procMsg);
        strcpy(p->procMsg,"");
    }
    else if(*currentPid != 0)
    {
        ListPrepend(receiveQueue,(void*)currentPid);
        pcbList[*currentPid]->procState = BLOCKED;
        printf("process %d blocked with receiving\n",*currentPid);
        printf("process %d quantum\n",*currentPid);
        currentPid = NULL;
        quantum();
    }
}

// unblocks sender and delivers reply
int reply(int pid,char* msg)
{
    PCB* pcb = pcbList[pid];
    ListFirst(sendQueue);
    int* p = (int*)ListSearch(sendQueue,isEqual,&pid);
    if(p != NULL)
    {
        strcpy(pcb->procMsg,msg);
        ListPrepend(readyQueue[pcb->priority],ListRemove(sendQueue));
        printf("reply to process %d\n",pid);
        printf("process %d unblocked with process %d\n",pid,*currentPid);
        return 0;
    }
    
    ListFirst(receiveQueue);
    p = (int*)ListSearch(receiveQueue,isEqual,&pid);
    if(p != NULL)
    {
        strcpy(pcb->procMsg,msg);
        ListPrepend(readyQueue[pcb->priority],ListRemove(receiveQueue));
        printf("reply to process %d\n",pid);
        printf("process %d unblocked with process %d\n",pid,*currentPid);
        return 0;
    }
    return 0;
}

//Initialize the named semaphore with the value given.
int newSemaphore(int id,int value)
{
    if(semaphores[id] == NULL)
    {
        semaphores[id] = (SEMAPHORE*)malloc(sizeof(SEMAPHORE));
        semaphores[id]->value = value;
        semaphores[id]->procList = ListCreate();
        printf("new Semaphore %d created with value %d.\n",id,value);
    }
    else
    {
        printf("new Semaphore failure,id %d has beed initialized.\n",id);    
    }
}

//execute the semaphore P operation.
int semaphoreP(int id)
{
    if(*currentPid !=0)
    {
        semaphores[id]->value -= 1;
        if(semaphores[id]->value < 0)
        {
            ListPrepend(semaphores[id]->procList,(void*)currentPid);
            pcbList[*currentPid]->procState = BLOCKED;
            printf("process %d block on semaphore %d\n",*currentPid,id);
            currentPid = NULL;
            quantum();
        }
    }
}

//execute the semaphore V operation.
int semaphoreV(int id)
{
    semaphores[id]->value +=1;
    if(semaphores[id]->value <= 0)
    {
        int* procId = ListTrim(semaphores[id]->procList);
        ListPrepend(readyQueue[pcbList[*procId]->priority],(void*)procId);
        pcbList[*procId]->procState = READY;
        printf("process %d unblock with process %d\n",*procId,*currentPid);
    }
    return 0;
}

//Print the process's info.
void processInfo(int pid)
{
    PCB* pcb = pcbList[pid];
    if(pcb != NULL)
    {
        printf("Process's pid: %d\n",pcb->pid);
        printf("Process's priority: %d\n",pcb->priority);
        if(pcb->procState == RUNNING)
        {
            printf("Process Running.\n");
        }
        else if(pcb->procState == READY)
        {
            printf("Process Ready.\n");
        }
        else if(pcb->procState == BLOCKED)
        {
            printf("Process Blocked.\n");
        }
        else
        {
            printf("Process exited.\n");
        }
    }
    else
    {
        printf("Process not exist.\n");
    }
}

// Display all process queues and their contents.
void totalInfo()
{
    for(int i = 0;i < 3;++i)
    {
        printf("ready queue priority %d\n",i);
        showQueue(readyQueue[i]);
    }
    
    printf("send waiting queue\n");
    showQueue(sendQueue);
    printf("receive waiting queue\n");
    showQueue(receiveQueue);

    for(int i = 0; i < 5;++i)
    {
        if(semaphores[i] != NULL)
        {
            printf("Semaphore: %d with value %d\n",i,semaphores[i]->value);
            showQueue(semaphores[i]->procList);
        }
    }
}
