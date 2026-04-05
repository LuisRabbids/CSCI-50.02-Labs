#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//entry function
void *monteCarlo(void *ptr);

struct ThreadData
{
    int threadNumber;       //thread number, ie thread 0, thread 1, thread 2, etc
    int threadTrials;       //number of trials in the thread
    int progressNumber;     //progress reporting number
    int insideCount;        //number of samples in the circle
};

int totalTrials = 0;        //total number of trials across all threads
int totalInside = 0;        //total number of samples in the circle across all threads

int main(int argc, char *argv[])
{
    int threadNum = atoi(argv[1]);
    int trials = atoi(argv[2]);
    int progressNum = atoi(argv[3]);

    pthread_t threads[threadNum];
    ThreadData threadData[threadNum];

    for(int i = 0; i < threadNum; i++)
    {
        threadData[i].threadNumber = i;
        threadData[i].threadTrials = trials;
        threadData[i].progressNumber = progressNum;
        threadData[i].insideCount = 0;

        if(pthread_create(&threads[i], NULL, monteCarlo, &threadData[i]))
        {
            fprintf(stderr,"Error - pthread_create()");
            exit(EXIT_FAILURE);
        }
    }

    for(int i = 0; i < threadNum; i++)
    {
        pthread_join(threads[i], NULL);
    }

    long double pi = 4.0 * totalInside / totalTrials;
    cout << "Final result: " << pi << endl;

    return 0;
}

