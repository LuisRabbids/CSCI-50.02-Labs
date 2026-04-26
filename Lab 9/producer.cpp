#include <iostream>
#include <string>
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
    int semId = semget(semKey, 1, IPC_CREAT | 0666);

    key_t shmKey = 5678;
    int shmId = shmget(shmKey, sizeof(SharedData), IPC_CREAT | 0666);
}