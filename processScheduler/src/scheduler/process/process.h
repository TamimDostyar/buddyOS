#ifndef PROCESS_U
#define PROCESS_U

typedef struct {
    int processState; // 1 for running, 0 for terminate, 2 waiting and 3 read
    int processNumber; // bascially the pid for each process
    
} Pid_t;



#endif