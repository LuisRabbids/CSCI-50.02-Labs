#include "IntArray.h"

void multiplyByX(IntArray *p, int x)
{
    int n = p->size;
    int *arr = p->elements;
    for (int i = 0; i < n; ++i) {
        arr[i] = arr[i] * x;
    }
}