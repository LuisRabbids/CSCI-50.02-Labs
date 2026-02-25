#include <iostream>
#include "IntArray.h"

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
    std::cout << "Before: ";
    for (int i = 0; i < A.size; ++i) std::cout << A.elements[i] << " ";
    std::cout << "\n";

    // - multiply it by a number of your choosing via multiplyByX()
    multiplyByX(&A, 67);

    // - print the result
    std::cout << "After:  ";
    for (int i = 0; i < A.size; ++i) std::cout << A.elements[i] << " ";
    std::cout << "\n";

    delete[] A.elements;
    return 0;
}