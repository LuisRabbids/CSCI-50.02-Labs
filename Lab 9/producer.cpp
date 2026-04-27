#include <iostream>
#include <string>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <fstream>
#include <cstdlib>

#include "shared.h"
using namespace std;

int main(int argc, char* argv[])
{
    if(argc != 3) 
    {
        cout << "./producer fileName fps" << endl;
        return 1;
    }

    string fileName = argv[1];
    int fps = atoi(argv[2]);

    if(fps <= 0)
    {
        cout << "fps must be positive" << endl;
        return 1;
    }

    int framerate = 1000000 / fps;      //microseconds for usleep

    key_t semKey = 1234;
    int semID = semget(semKey, 1, IPC_CREAT | 0666);

    key_t shmKey = 5678;
    int shmID = shmget(shmKey, 1 << 10, IPC_CREAT | 0666);

    SharedData* data = (SharedData*)shmat(shmID, NULL, 0);
    data->frameNumber = 0;
    data->totalFrames = 0;

    ifstream file(fileName);
    string line = "";
    string frame = "";
    vector<string> frames;

    int frameCount = 0;

    while(getline(file, line))
    {
        if(!line.empty() && line[0] == '\066')
        {
            frameCount++;

            if(!frame.empty())      //new frame
            {
                frames.push_back(frame);
                frame.clear();
            }
        }
        else
        {
            frame += line + "\n";
        }
    }
    if(!frame.empty())              //push last frame
    {
        frames.push_back(frame);
        frame.clear();
    }

    data->totalFrames = frameCount;

    int index = 0;

    while(true)
    {
        wait(semID);

        strcpy(data->frame, frames[index].c_str());
        data->frameNumber = index + 1;
        data->newFrame = 1;

        signal(semID);

        index = (index + 1) % frameCount;

        usleep(framerate);
    }
}