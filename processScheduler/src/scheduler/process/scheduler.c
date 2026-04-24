#include "scheduler.h"
#include "process.h"

static inline void init_scheduler(QueueScheduler *scheduler) {
    scheduler->queueHead = 0;
    scheduler->queueTail = 0;
    scheduler->queueSize = 0;
    scheduler->currentPID = -1;
}

static inline int enqueue(QueueScheduler *scheduler, int pid) {
    if (scheduler->queueSize >= MAX_QUEUE_SIZE) {
        return -1;  // Queue full
    }
    
    scheduler->readyQueue[scheduler->queueTail] = pid;
    scheduler->queueTail = (scheduler->queueTail + 1) % MAX_QUEUE_SIZE;
    scheduler->queueSize++;
    
    return 0;
}

static inline int dequeue(QueueScheduler *scheduler) {
    if (scheduler->queueSize == 0) {
        return -1;  // No process ready
    }
    
    int pid = scheduler->readyQueue[scheduler->queueHead];
    scheduler->queueHead = (scheduler->queueHead + 1) % MAX_QUEUE_SIZE;
    scheduler->queueSize--;
    
    return pid;
}

// Schedule next process to run
static inline int schedule(QueueScheduler *scheduler) {
    int nextPID = dequeue(scheduler);
    
    if (nextPID == -1) {
        return -1;
    }
    
    scheduler->currentPID = nextPID;
    
    return nextPID;
}