#ifndef SCHEDULER
#define SCHEDULER


typedef struct{
    // where the main queue lives
    int currnetOnes; //basically from 1 - N where if this number is 5 that means queue has 5 one to go
    int controlUnit; // control unit basically meaning the stage it is at
    int outPut;  // based on the control unit any updates?

} QueueScheduler;


#endif