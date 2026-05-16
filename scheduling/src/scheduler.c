#include "scheduler.h"
#include "process.h"
#include <stdint.h>
#include <stddef.h>

extern void     __task_set_current(int pid);
extern Process *__scheduler_lookup(int pid);   /* defined in process.c */

/* ------------------------------------------------------------------ */
/* Legacy round-robin queue (unchanged API)                           */
/* ------------------------------------------------------------------ */

void init_scheduler(QueueScheduler *s) {
    s->queueHead = s->queueTail = s->queueSize = 0;
    s->currentPID = -1;
}

int enqueue(QueueScheduler *s, int pid) {
    if (s->queueSize >= MAX_QUEUE_SIZE) return -1;
    s->readyQueue[s->queueTail] = pid;
    s->queueTail = (s->queueTail + 1) % MAX_QUEUE_SIZE;
    s->queueSize++;
    return 0;
}

int dequeue(QueueScheduler *s) {
    if (s->queueSize == 0) return -1;
    int pid = s->readyQueue[s->queueHead];
    s->queueHead = (s->queueHead + 1) % MAX_QUEUE_SIZE;
    s->queueSize--;
    return pid;
}

int schedule(QueueScheduler *s) {
    int p = dequeue(s);
    if (p == -1) return -1;
    s->currentPID = p;
    return p;
}

/* ------------------------------------------------------------------ */
/* Active scheduler: a single global ring of pids                      */
/* ------------------------------------------------------------------ */

static QueueScheduler g_sched;
static int            g_in_switch  = 0;
static uint32_t       g_tick_count = 0;
#define TICKS_PER_SLICE 1

void scheduler_init(void) {
    init_scheduler(&g_sched);
}

void scheduler_add(Process *p) {
    if (!p) return;
    /* Avoid double-enqueue. */
    int i = g_sched.queueHead;
    int n = g_sched.queueSize;
    while (n-- > 0) {
        if (g_sched.readyQueue[i] == p->pid) return;
        i = (i + 1) % MAX_QUEUE_SIZE;
    }
    enqueue(&g_sched, p->pid);
    if (p->processState != PROCESS_RUNNING) p->processState = PROCESS_READY;
}

void scheduler_remove(Process *p) {
    if (!p || g_sched.queueSize == 0) return;
    int kept = 0;
    int tmp[MAX_QUEUE_SIZE];
    int idx = g_sched.queueHead;
    int n   = g_sched.queueSize;
    while (n-- > 0) {
        if (g_sched.readyQueue[idx] != p->pid) tmp[kept++] = g_sched.readyQueue[idx];
        idx = (idx + 1) % MAX_QUEUE_SIZE;
    }
    g_sched.queueHead = 0;
    g_sched.queueTail = kept;
    g_sched.queueSize = kept;
    for (int i = 0; i < kept; i++) g_sched.readyQueue[i] = tmp[i];
}

/* Pick the next runnable pid that's not self. -1 if none. */
static int pick_next(int self) {
    int tries = g_sched.queueSize;
    while (tries-- > 0) {
        int pid = dequeue(&g_sched);
        if (pid < 0) return -1;

        Process *cand = __scheduler_lookup(pid);
        if (!cand) continue;

        if (cand->processState == PROCESS_ZOMBIE  ||
            cand->processState == PROCESS_FREE    ||
            cand->processState == PROCESS_WAITING) {
            /* Drop from queue — not runnable right now. */
            continue;
        }

        /* Re-queue at tail for round robin. */
        enqueue(&g_sched, pid);
        if (pid != self) return pid;
    }
    return -1;
}

void scheduler_yield(void) {
    if (g_in_switch) return;
    g_in_switch = 1;

    int self = task_current_pid();
    int next = pick_next(self);
    if (next < 0 || next == self) { g_in_switch = 0; return; }

    Process *cur = task_current();
    Process *nxt = __scheduler_lookup(next);
    if (!nxt) { g_in_switch = 0; return; }

    if (cur->processState == PROCESS_RUNNING) cur->processState = PROCESS_READY;
    nxt->processState = PROCESS_RUNNING;

    __task_set_current(next);
    g_in_switch = 0;
    context_switch(&cur->esp, nxt->esp);
    /* Resumes here when this task is scheduled back in. */
}

void scheduler_on_tick(void) {
    g_tick_count++;
    if ((g_tick_count % TICKS_PER_SLICE) == 0) scheduler_yield();
}
