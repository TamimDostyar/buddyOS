#ifndef SCHEDULER
#define SCHEDULER

#include "process.h"

#define MAX_QUEUE_SIZE 1024

typedef struct {
    int readyQueue[MAX_QUEUE_SIZE];
    int queueHead;                
    int queueTail;               
    int queueSize;              
    int currentPID;              
} QueueScheduler;

#endif