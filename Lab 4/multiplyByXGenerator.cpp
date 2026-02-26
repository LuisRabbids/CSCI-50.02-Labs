#include <iostream>
#include "IntArray.h"
using namespace std;

int main(int argc, char *argv[])
{
    string input = argv[1];
    int x = atoi(argv[1]);

    cout << "\t.file\t\"multiplyBy" << x <<".cpp\"" << endl;
    cout << "\t.text" << endl;
    cout << "\t.globl\t_Z" << 10 + input.length() << "multiplyBy" << x << "P8IntArrayi" << endl;
    cout << "\t.type\t_Z" << 10 + input.length() << "multiplyBy" << x << "P8IntArrayi, @function" << endl;
    cout << "_Z" << 10 + input.length() << "multiplyByXP8IntArrayi:" << endl;

    cout << ".LFB0:" << endl;
    cout << "\t.cfi_startproc" << endl;
    cout << "\tendbr64" << endl;

    cout << "\tpushq\t%rbp" << endl;
    
    return 0;
}

// https://learn.microsoft.com/en-us/cpp/cpp/main-function-command-line-args?view=msvc-170
// https://www.geeksforgeeks.org/cpp/cpp-program-for-char-to-int-conversion/
// https://itanium-cxx-abi.github.io/cxx-abi/abi.html#mangling