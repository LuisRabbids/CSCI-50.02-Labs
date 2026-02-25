#include <iostream>
#include "IntArray.h"
using namespace std;

int main(char *argv[])
{
    int x = argv[0]

    cout << "\t.file\t\"multiplyBy" << x <<".cpp\"" << endl;
    cout << "\t.text" << endl;
    cout << "\t.globl _Z11multiplyBy" << x << "P8IntArrayi" << endl;
    cout << "\t.globl _Z11multiplyBy" << x << "P8IntArrayi, @function" << endl; 
}