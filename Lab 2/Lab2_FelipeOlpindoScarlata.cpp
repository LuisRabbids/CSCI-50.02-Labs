#include <iostream>
#include <string>
using namespace std;

bool isNum(string line)
{
    char* p;
    strtol(line.c_str(), &p, 10);
    return *p == 0;
}

int main(void){
    int numAgents;
    cin >> numAgents;

    for (int i = 1; i-1 < numAgents; i++){
        int x, y;
        cin >> x >> y;              // read in the coordinates of the current agent
        cin.ignore(1, ' ');         // clean up the single space left after 'y'

        string message;
        getline(cin, message);      // get the rest of the line as the message

        cout << "Agent #" << i << " is at (" << x << ", " << y << ")" << endl;      // print the coordinate of current agent

        //UPDATE TO TAKE INTO ACCOUNT LARGER NUMBERS (multiple digits)
        //update to check if message is a number
        if ( isNum(message)) {                                               // check if the message length is 1 (which is not always so UPDATE)
            cout << "Agent #" << i << " holds up the number: " << message << endl;  // print number held up by agent
        } else {
            cout << "Agent #" << i << " yells: \"" << message << "\"" << endl;      // print message yelled by agent
        }
    }

    return 0;
}