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

/* Raw queue ops (used by the active scheduler below). */
void init_scheduler(QueueScheduler *s);
int  enqueue(QueueScheduler *s, int pid);
int  dequeue(QueueScheduler *s);
int  schedule(QueueScheduler *s);

void scheduler_init(void);
void scheduler_add(Process *p);
void scheduler_remove(Process *p);
void scheduler_on_tick(void);
void scheduler_yield(void);

/* switch.s */
extern void context_switch(uint32_t *save_old_esp, uint32_t new_esp);

#endif
