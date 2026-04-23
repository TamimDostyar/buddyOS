#include "process.h"

Pid_t pid_t;


static inline int fork(){
    int currentPID = pid_t.processNumber;
    if (currentPID == 0){
        exit();
        return 0;

    } else {
        /
        currentPID ++;
    }
    // meaning it is running
    int pid_t.processState = 1;
}


static inline void exit(){
    int currentStage = pid_t.processState;
    int currentPID = pid_t.processNumber;


    // if it is running then fine
    if (currentStage == 0){
        continue;
    } else if( currentStage == 1){
        // wait like a few seconds if the program stays then terminate
        wait(2000);
        killPID(current_PID);
        // return 0;
    };
    // assiging to 0 that stage meaing it is running
    int currentStage = 0;
}



// simply exist
static void killPID(int pid){
    exit(pid);
}

// sleep
int wait(int seconds){
    sleep(seconds);
}

static inline void exec(int process){
    // since processNumber gets replaced we can exchange?
    pid_t.processNumber = process;

}


// int main(){
//     wait();
//     return 0;
// }