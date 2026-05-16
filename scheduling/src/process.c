#include "process.h"
#include "scheduler.h"
#include "heap.h"
#include <stdint.h>
#include <stddef.h>

static Process tasks[MAX_PROCESSES];
static int     current_pid_v = 0;

Process *task_current(void)     { return &tasks[current_pid_v]; }
int      task_current_pid(void) { return current_pid_v; }

void __task_set_current(int pid) { current_pid_v = pid; }

Process *__scheduler_lookup(int pid) {
    if (pid < 0 || pid >= MAX_PROCESSES) return (Process *)0;
    return &tasks[pid];
}

/* Lands here when an entry function returns normally. */
static void task_return_trampoline(void) {
    task_exit(0);
    for (;;) __asm__ volatile("cli; hlt");
}

void task_system_init(void) {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        tasks[i].pid          = i;
        tasks[i].processState = PROCESS_FREE;
        tasks[i].kstack       = (void *)0;
        tasks[i].esp          = 0;
        tasks[i].parentPID    = -1;
        tasks[i].exit_status  = 0;
        tasks[i].name[0]      = '\0';
    }

    /* PID 0 is the running kmain context. Its stack is the boot stack
       from entry.asm; esp gets saved on the first switch away. */
    tasks[0].processState = PROCESS_RUNNING;
    tasks[0].name[0] = 'k'; tasks[0].name[1] = 'm';
    tasks[0].name[2] = 'a'; tasks[0].name[3] = 'i';
    tasks[0].name[4] = 'n'; tasks[0].name[5] = '\0';

    current_pid_v = 0;
    scheduler_init();
    scheduler_add(&tasks[0]);
}

Process *task_create(const char *name, task_entry_t entry) {
    int slot = -1;
    for (int i = 1; i < MAX_PROCESSES; i++) {
        if (tasks[i].processState == PROCESS_FREE) { slot = i; break; }
    }
    if (slot < 0) return (Process *)0;

    Process *p = &tasks[slot];

    void *st = kmalloc(KSTACK_SIZE);
    if (!st) return (Process *)0;
    p->kstack = st;

    /* Fake the frame context_switch expects: it will popfl, pop
       edi/esi/ebx/ebp, then ret into entry. If entry returns, the
       trampoline below it gets popped and runs task_exit. */
    uint32_t *sp = (uint32_t *)((char *)st + KSTACK_SIZE);
    *--sp = (uint32_t)task_return_trampoline;
    *--sp = (uint32_t)entry;
    *--sp = 0;                  /* ebp */
    *--sp = 0;                  /* ebx */
    *--sp = 0;                  /* esi */
    *--sp = 0;                  /* edi */
    *--sp = 0x00000202;         /* eflags: IF=1, reserved bit 1 */
    p->esp = (uint32_t)sp;

    p->parentPID    = current_pid_v;
    p->processState = PROCESS_READY;
    p->exit_status  = 0;

    int i;
    for (i = 0; i < 15 && name && name[i]; i++) p->name[i] = name[i];
    p->name[i] = '\0';

    scheduler_add(p);
    return p;
}

/* Not a real fork: child starts at child_entry, not at the parent's PC.
   Good enough for fork-then-exec; full image copy needs paging. */
int task_fork(task_entry_t child_entry) {
    Process *child = task_create("forked", child_entry);
    if (!child) return -1;
    return child->pid;
}

void task_exit(int status) {
    Process *me = &tasks[current_pid_v];
    me->exit_status  = status;
    me->processState = PROCESS_ZOMBIE;

    if (me->parentPID >= 0 && me->parentPID < MAX_PROCESSES) {
        Process *parent = &tasks[me->parentPID];
        if (parent->processState == PROCESS_WAITING) {
            parent->processState = PROCESS_READY;
            scheduler_add(parent);
        }
    }

    scheduler_yield();
    for (;;) __asm__ volatile("cli; hlt");      /* unreachable */
}

int task_wait(int *out_status) {
    Process *me = &tasks[current_pid_v];

    for (;;) {
        for (int i = 1; i < MAX_PROCESSES; i++) {
            if (tasks[i].parentPID    == current_pid_v &&
                tasks[i].processState == PROCESS_ZOMBIE) {

                int cpid = tasks[i].pid;
                if (out_status) *out_status = tasks[i].exit_status;

                if (tasks[i].kstack) {
                    kfree(tasks[i].kstack);
                    tasks[i].kstack = (void *)0;
                }
                tasks[i].processState = PROCESS_FREE;
                tasks[i].parentPID    = -1;
                return cpid;
            }
        }

        me->processState = PROCESS_WAITING;
        scheduler_yield();
    }
}

void task_yield(void) { scheduler_yield(); }

/* --- pre-task-layer shims; new callers should use task_*. --- */

int fork(Pid_t *p, int parentPID) {
    (void)p; (void)parentPID;
    return -1;                  /* loud failure if anyone still calls this */
}

int exit_process(Pid_t *p, int pid) {
    (void)p;
    if (pid <= 0 || pid >= MAX_PROCESSES) return -1;
    tasks[pid].processState = PROCESS_FREE;
    return 0;
}

int wait(Pid_t *p, int parentPID) {
    (void)p; (void)parentPID;
    return task_wait(NULL);
}
