#include <iostream>
#include <unistd.h>
using namespace std;

int main()
{
    int pid = 1;
    int pid2 = 1;
    int pid3 = 1;
    int pid4 = 1;
    int pid5 = 1;

    cout << "start" << endl;
    if ( (pid = fork()) == 0 ) 
    {
    // process 1
    cout << "process 1: " << pid << ", " << pid2 << ", " << pid3 << ", " << pid4 << ", " << pid5 << " end" << endl;
    cout << "1" << endl;
    } 
    else if ( (pid2 = fork()) == 0 ) 
    {
    // process 2
    cout << "process 2: " << pid << ", " << pid2 << ", " << pid3 << ", " << pid4 << ", " << pid5 << " end" << endl;
    cout << "2" << endl;
    } 
    else if ( (pid3 = fork()) == 0 ) 
    {
    // process 3
    cout << "process 3: " << pid << ", " << pid2 << ", " << pid3 << ", " << pid4 << ", " << pid5 << " end" << endl;
    cout << "3" << endl;
    } 
    else if ( (pid4 = fork()) == 0 ) 
    {
    // process 4
    cout << "process 4: " << pid << ", " << pid2 << ", " << pid3 << ", " << pid4 << ", " << pid5 << " end" << endl;
    cout << "4" << endl;
    } 
    else if ( (pid5 = fork()) == 0 ) 
    {
    // process 5
    cout << "process 5: " << pid << ", " << pid2 << ", " << pid3 << ", " << pid4 << ", " << pid5 << " end" << endl;
    cout << "5" << endl;
    } 
    else 
    {
    // process 6
    cout << "process 6: " << pid << ", " << pid2 << ", " << pid3 << ", " << pid4 << ", " << pid5 << " end" << endl;
    cout << "6" << endl;
    }

return 0;
}