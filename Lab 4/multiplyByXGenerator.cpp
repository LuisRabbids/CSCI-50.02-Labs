#include <iostream>
#include "IntArray.h"
using namespace std;

int main(int argc, char *argv[])
{
    int x = atoi(argv[1]);

    cout << "\t.file\t\"multiplyBy" << x <<".cpp\"" << endl;
    cout << "\t.text" << endl;
    cout << "\t.globl\t_Z11multiplyBy" << x << "P8IntArrayi" << endl;
    cout << "\t.type\t_Z11multiplyBy" << x << "P8IntArrayi, @function" << endl;
    cout << "_Z11multiplyByXP8IntArrayi:" << endl;

    cout << ".LFB0:" << endl;
    cout << "\t.cfi_startproc" << endl;
    cout << "\tendbr64" << endl;
    
    return 0;
}

// https://learn.microsoft.com/en-us/cpp/cpp/main-function-command-line-args?view=msvc-170
// https://www.geeksforgeeks.org/cpp/cpp-program-for-char-to-int-conversion/