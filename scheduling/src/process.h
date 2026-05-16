#ifndef PROCESS_H
#define PROCESS_H

#include "heap.h"
#include <stdint.h>

#define MAX_PROCESSES   64
#define KSTACK_SIZE     8192

#define PROCESS_FREE     0
#define PROCESS_RUNNING  1
#define PROCESS_READY    2
#define PROCESS_WAITING  3
#define PROCESS_ZOMBIE   4

typedef struct process {
    int       pid;
    int       parentPID;
    int       processState;
    int       exit_status;
    uint32_t  esp;           
    void     *kstack;        
    char      name[16];
} Process;

/* Legacy "Pid_t" wrapper kept for backwards compatibility. */
typedef struct {
    Process *processTable;
    int      slotsTaken[MAX_PROCESSES];
    int      processSlot;
    int      totalSlots;
} Pid_t;

int fork(Pid_t *pid_t, int parentPID);
int exit_process(Pid_t *manager, int pid);
int wait(Pid_t *manager, int parentPID);


typedef void (*task_entry_t)(void);

void     task_system_init(void);
Process *task_current(void);
int      task_current_pid(void);
Process *task_create(const char *name, task_entry_t entry);
int      task_fork(task_entry_t child_entry);
void     task_exit(int status);
int      task_wait(int *out_status);
void     task_yield(void);

#endif
