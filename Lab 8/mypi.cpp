#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//entry function
void *monteCarlo(void *ptr);

struct ThreadStuff // rename
{
    int threadNumber;       //thread number, ie thread 0, thread 1, thread 2, etc
    int threadTrials;       //number of trials in the thread
    int progressNumber;     //progress reporting number
    int insideCount;        //number of samples in the circle
};

