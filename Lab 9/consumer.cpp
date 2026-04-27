#include <iostream>
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
    int semId = semget(semKey, 1, IPC_CREAT | 0666);

    key_t shmKey = 5678;
    int shmId = shmget(shmKey, 1 << 10, IPC_CREAT | 0666);

    SharedData* data = (SharedData*)shmat(shmId, NULL, 0);
}