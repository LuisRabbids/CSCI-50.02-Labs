#include <iostream>
#include "IntArray.h"
using namespace std;

extern void multiplyByX(IntArray *p, int x);


int main() {
    // - create a new IntArray with your own elements
    IntArray A;
    A.size = 10;
    A.elements = new int[A.size];

    for (int i = 0; i < A.size; ++i) {
        A.elements[i] = i + 1;
    }   

    // - print it out
    cout << "Before: ";
    for (int i = 0; i < A.size; ++i) cout << A.elements[i] << " ";
    cout << "\n";

    // - multiply it by a number of your choosing via multiplyByX()
    multiplyByX(&A, 67);

    // - print the result
    cout << "After:  ";
    for (int i = 0; i < A.size; ++i) cout << A.elements[i] << " ";
    cout << "\n";

    delete[] A.elements;
    A.elements = NULL;
    
    return 0;
}