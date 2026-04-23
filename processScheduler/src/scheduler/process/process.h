#include "heap.h"




typedef struct {
    // int processState; // 1 for running, 0 for terminate, 2 waiting and 3 read
    // int processState[MAX_PRCESS]
    int *processTable; // dynamically allocate to avoid memory limit
    int processNumber; // bascially the pid for each process
    
} Pid_t;

