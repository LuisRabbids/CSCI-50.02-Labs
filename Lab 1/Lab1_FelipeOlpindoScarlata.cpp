#include <iostream>
using namespace std;


int main(void){
    string name;                                          // instantiate a string variable
    cout << "What is your name?: ";                       // prompt and ask user for their name         
    cin >> name;                                          // read user's input
    cout << name << endl;                                 // print out the name


    cout << name[0];                                      // print out the first letter of the name               
    for (int i = 0; i < name.length() - 1; i++) {         // loop through the rest of the letters
        cout << "#";                                      // print out "#" for each remaining letter
    }
    cout << endl;                                         // print a newline


    // 4. Print out the user’s name with all the letters replaced with the next letter


    // 5. Function that uses a character pointer argument 

    return 0;
}
