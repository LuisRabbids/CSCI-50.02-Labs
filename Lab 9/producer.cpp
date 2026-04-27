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
#include <pthread.h>

#include "shared.h"
using namespace std;

bool running = true;                // set to false when the user presses Enter

void* exitWhenEnter(void* arg)      // entry function for the thread that listens for Enter
{
    cin.get();                      // blocks here until user presses Enter
    running = false;
    return NULL;
}

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

    semctl(semID, 0, SETVAL, 1);

    key_t shmKey = 5678;
    int shmID = shmget(shmKey, 65536, IPC_CREAT | 0666);

    SharedData* data = (SharedData*)shmat(shmID, NULL, 0);
    data->frameNumber = 0;
    data->totalFrames = 0;

    ifstream file(fileName);
    string line = "";
    string frame = "";
    vector<string> frames;

    int frameCount = 0;

    //reads through each 'frame' in the file
    while(getline(file, line))
    {
        if(!line.empty() && line[0] == '\033')
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

    // create a thread that listens for Enter in the background while the main loop keeps running
    pthread_t inputThread;
    pthread_create(&inputThread, NULL, exitWhenEnter, NULL);

    while(running)          // exits when Enter is pressed
    {
        wait(semID);

        strcpy(data->frame, frames[index].c_str());     //copies the frame in the vector into the shared data
        data->frameNumber = index + 1;
        data->newFrame = 1;

        signal(semID);

        index = (index + 1) % frameCount;               //resets the index to 0 if it reaches the last frame

        usleep(framerate);
    }

    return 0;
}