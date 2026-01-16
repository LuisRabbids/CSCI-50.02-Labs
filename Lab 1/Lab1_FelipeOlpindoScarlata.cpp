#include <iostream>
using namespace std;

void shiftFunc(char* x){                                // function to replace a character with the next letter
    if (*x >= 'A' && *x <= 'Z') {                       // check if character is uppercase
        *x = (*x + 1 - 65) % 26 + 65;                   // add one to the character and wrap around if necessary using modulo % 26 because there are 26 alphabets, and - 65 (accounting for ASCII which starts with 0) since 'A' is the 66th in ASCII
    }

    else if (*x >= 'a' && *x <= 'z') {                  // check if character is lowercase
        *x = (*x + 1 - 97) % 26 + 97;                   // add one to the character and wrap around if necessary using modulo % 26 because there are 26 alphabets, and - 97 (accounting for ASCII which starts with 0) since 'a' is the 98th in ASCII
    }                          
}

int main(void){
    //1 and 2. Ask user for their name and print it out
    string name, result = "";       //string to store the users input
    cout << "Enter your name: ";    // asks for the user's name
    getline(cin, name);             // reads entire line

    //removes whitespace characters
    for (char c : name) {            // loop through each character
        if (c != ' ') {              //check if character is not a whitespace
            result += c;             //print if the character if its not a whitespace character
        }
    }
    cout << result << endl;    // print name without spaces


    //3. Print user's name with first letter revealed and rest as "#"
    cout << name[0];                                      // print out the first letter of the name               
    for (int i = 0; i < name.length() - 1; i++) {         // loop through the rest of the letters
        cout << "#";                                      // print out "#" for each remaining letter
    }
    cout << endl;                                         // print a newline


    // 4. Print out the user’s name with all the letters replaced with the next letter
    for (char c : name) {                                 // loop through each character in the name
        if (c >= 'A' && c <= 'Z') {                       // check if character is uppercase
            c = (c + 1 - 65) % 26 + 65;                   // add one to the character and wrap around if necessary using modulo % 26 because there are 26 alphabets, and - 65 (accounting for ASCII which starts with 0) since 'A' is the 66th in ASCII
        }
        else if (c >= 'a' && c <= 'z') {                  // check if character is lowercase
            c = (c + 1 - 97) % 26 + 97;                   // add one to the character and wrap around if necessary using modulo % 26 because there are 26 alphabets, and - 97 (accounting for ASCII which starts with 0) since 'a' is the 98th in ASCII
        }
        cout << c;                                        // print the modified character                          
    }
    cout << endl;                                         // print a newline


    // 5. Function that uses a character pointer argument 
    for (int i = 0; i < name.length(); i++)             // loop through the length of the given string to go through all its characters
    {
        shiftFunc(&name[i]);                            // call the function on each character in the given string
    }
    cout << name << endl;                               // print the modified string
    
    return 0;
}
