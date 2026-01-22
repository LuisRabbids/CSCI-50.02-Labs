#include <iostream>
#include <string>
using namespace std;

bool isNum(string line)             // boolean function to check if the given string is an integer
{
    char* p;
    strtol(line.c_str(), &p, 10);   // converts the string into an integer of base 10 and puts into p the value of the rest of the string after the first integer
    return *p == 0;                 // true if the string is only an integer, false if it is multiple integers or has a non-integer data type in it
}

int main(void){
    int numAgents;
    cin >> numAgents;

    for (int i = 1; i-1 < numAgents; i++){          // start at 1 because the Agent count starts at 1
        int x, y;
        cin >> x >> y;              // read in the coordinates of the current agent
        cin.ignore(1, ' ');         // clean up the single space left after 'y'

        string message;
        getline(cin, message);      // get the rest of the line as the message

        cout << "Agent #" << i << " is at (" << x << ", " << y << ")" << endl;      // print the coordinate of current agent

        if ( isNum(message)) {                                                      // check if the message is only an integer
            cout << "Agent #" << i << " holds up the number: " << message << endl;  // print number held up by agent
        } else {
            cout << "Agent #" << i << " yells: \"" << message << "\"" << endl;      // print message yelled by agent
        }
    }

    return 0;
}