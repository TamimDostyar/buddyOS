#include "process.h"
#include "scheduler.h"
#include "heap.h"
#include <stdint.h>
#include <stddef.h>


static Process tasks[MAX_PROCESSES];
static int     current_pid_v = 0;

Process *task_current(void)          { return &tasks[current_pid_v]; }
int      task_current_pid(void)      { return current_pid_v; }

/* Used by scheduler.c to flip the current pointer atomically with context_switch. */
void __task_set_current(int pid) { current_pid_v = pid; }

Process *__scheduler_lookup(int pid) {
    if (pid < 0 || pid >= MAX_PROCESSES) return (Process *)0;
    return &tasks[pid];
}


/* If a task's entry function returns, control lands here. */
static void task_return_trampoline(void) {
    task_exit(0);
    for (;;) { __asm__ volatile("cli; hlt"); }
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

    /* Slot 0 = the running kmain context. We don't own its stack; it's
       the boot stack set up in entry.asm (0x9C000). esp will be saved
       the first time we switch away from it. 
    */
   // below the feature slots
    tasks[0].processState = PROCESS_RUNNING;
    tasks[0].name[0] = 'k'; tasks[0].name[1] = 'm';
    tasks[0].name[2] = 'a'; tasks[0].name[3] = 'i';
    tasks[0].name[4] = 'n'; tasks[0].name[5] = '\0';

    current_pid_v = 0;
    scheduler_init();
    scheduler_add(&tasks[0]);
}


Process *task_create(const char *name, task_entry_t entry) {
    /* Find a free slot (start at 1 — slot 0 is kmain). */

    int slot = -1;
    for (int i = 1; i < MAX_PROCESSES; i++) {
        if (tasks[i].processState == PROCESS_FREE) { slot = i; break; }
    }
    if (slot < 0) return (Process *)0;

    Process *p = &tasks[slot];

    /* Allocate stack */
    void *st = kmalloc(KSTACK_SIZE);
    if (!st) return (Process *)0;
    p->kstack = st;

    /* Build initial frame at the top of the stack. context_switch will
       pop: eflags, edi, esi, ebx, ebp, ret. So push in reverse order:
       trampoline, entry, ebp, ebx, esi, edi, eflags.                  
    */
    uint32_t *sp = (uint32_t *)((char *)st + KSTACK_SIZE);
    *--sp = (uint32_t)task_return_trampoline;   
    *--sp = (uint32_t)entry;                    
    *--sp = 0;                                  
    *--sp = 0;                                  
    *--sp = 0;                                  
    *--sp = 0;                                  
    *--sp = 0x00000202;                         
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


/* True kernel-thread fork: child starts at child_entry, not at the
   parent's PC. Real Unix fork (copy-on-write image) needs paging; this
   is sufficient for a multitasking shell that does fork()+exec() in
   sequence. 
*/
int task_fork(task_entry_t child_entry) {
    Process *child = task_create("forked", child_entry);
    if (!child) return -1;
    return child->pid;
}

void task_exit(int status) {
    Process *me = &tasks[current_pid_v];
    me->exit_status  = status;
    me->processState = PROCESS_ZOMBIE;

    /* Wake parent if it's waiting on any child. */
    if (me->parentPID >= 0 && me->parentPID < MAX_PROCESSES) {
        Process *parent = &tasks[me->parentPID];
        if (parent->processState == PROCESS_WAITING) {
            parent->processState = PROCESS_READY;
            scheduler_add(parent);
        }
    }

    scheduler_yield();          /* never returns */
    for (;;) { __asm__ volatile("cli; hlt"); }
}

int task_wait(int *out_status) {
    Process *me = &tasks[current_pid_v];

    for (;;) {
        /* Look for a zombie child. */
        for (int i = 1; i < MAX_PROCESSES; i++) {
            if (tasks[i].parentPID    == current_pid_v &&
                tasks[i].processState == PROCESS_ZOMBIE) {

                int cpid = tasks[i].pid;
                if (out_status) *out_status = tasks[i].exit_status;

                /* Reap. */
                if (tasks[i].kstack) {
                    kfree(tasks[i].kstack);
                    tasks[i].kstack = (void *)0;
                }
                tasks[i].processState = PROCESS_FREE;
                tasks[i].parentPID    = -1;
                return cpid;
            }
        }

        /* No zombie yet — block. */
        me->processState = PROCESS_WAITING;
        scheduler_yield();
        /* On resume, parent is READY/RUNNING again, retry the scan. */
    }
}

void task_yield(void) { scheduler_yield(); }


int fork(Pid_t *p, int parentPID) {
    (void)p; (void)parentPID;
    /* Forward to the new layer; child entry must be wired by caller via
       task_create — return -1 so any stale callsite is obvious.        */
    return -1;
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
