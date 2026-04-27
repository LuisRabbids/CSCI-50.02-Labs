#include <iostream>
#include <string>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>

#include "shared.h"
using namespace std;

int main(int argc, char* argv[])
{
    if(argc != 2) 
    {
        cout << "./consumer fps" << endl;
        return 1;
    }

    int fps = atoi(argv[1]);

    bool sync = false;
    int framerate;
    if(fps < 0)
    {
        cout << "fps must be positive or 0" << endl;
        return 1;
    }
    else if(fps == 0)
    {
        sync = true;
    }
    else
    {
        framerate = 1000000 / fps;
    }

    key_t semKey = 1234;
    int semID = semget(semKey, 1, IPC_CREAT | 0666);

    key_t shmKey = 5678;
    int shmID = shmget(shmKey, 1 << 10, IPC_CREAT | 0666);

    SharedData* data = (SharedData*)shmat(shmID, NULL, 0);

    int previousFrame = -1;
    int skippedFrames = 0;

    while(true)
    {
        if(sync)
        {
            while(data->newFrame == 0);
        }
        else
        {
            usleep(framerate);
        }

        wait(semID);

        if(data->frameNumber != previousFrame + 1)
        {
            skippedFrames += (data->frameNumber - previousFrame - 1);
        }

        printf("\033c");
        printf("%s", data->frame);
        printf("Current frame: %d / %d (%d frame(s) skipped)\n", data->frameNumber, data->totalFrames, skippedFrames);

        previousFrame = data->frameNumber;
        data->newFrame = 0;

        signal(semID);
    }
}