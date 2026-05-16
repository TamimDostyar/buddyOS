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

/* ---------- legacy round-robin queue API ---------- */
void init_scheduler(QueueScheduler *scheduler);
int  enqueue(QueueScheduler *scheduler, int pid);
int  dequeue(QueueScheduler *scheduler);
int  schedule(QueueScheduler *scheduler);

/* ---------- new active scheduler API ---------- */
void scheduler_init(void);
void scheduler_add(Process *p);
void scheduler_remove(Process *p);
void scheduler_on_tick(void);
void scheduler_yield(void);

/* Implemented in switch.s — swaps esp + callee-saved regs. */
extern void context_switch(uint32_t *save_old_esp, uint32_t new_esp);

#endif
