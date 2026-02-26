#include <iostream>
#include "IntArray.h"
using namespace std;


extern void multiplyBy61(IntArray *p);


int main(){
    // Create a new IntArray
    IntArray A;
    A.size = 10;
    A.elements = new int[A.size];

    // Populate it with some values
    for (int i = 0; i < A.size; ++i) {
        A.elements[i] = i + 1;
    }   

    // Print the array before multiplying
    cout << "Before: ";
    for (int i = 0; i < A.size; ++i) {
        cout << A.elements[i] << " ";
    }
    cout << "\n";

    // Call generated assembly function, pass the struct pointer only
    multiplyBy61(&A);

    // Print the result after multiplying
    cout << "After multiplying by 61: ";
    for (int i = 0; i < A.size; ++i) {
        cout << A.elements[i] << " ";
    }
    cout << "\n";

    delete[] A.elements;
    A.elements = NULL;

    return 0;
}