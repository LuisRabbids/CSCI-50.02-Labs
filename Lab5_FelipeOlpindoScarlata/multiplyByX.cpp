#include "IntArray.h"

void multiplyByX(IntArray *p, int x)
{
    int n = p->size;                 // Get the size of the array from the struct
    int *arr = p->elements;          // Get the pointer to the actual integer array from the struct
    for (int i = 0; i < n; ++i) {
        arr[i] = arr[i] * x;         // Multiply current element by x and assign it back to overwrite the old value
    }
}