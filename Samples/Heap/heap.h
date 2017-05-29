#ifndef HEAP_H
#define HEAP_H
#include "stdlib.h"

class Heap
{
public:
    Heap(int size);
    ~Heap();

    void add(int elem);
    void print();

private:

    int*    heapArray;
    int     length;
    int     currentSize;

    void  siftUp(int index);
    void  siftDown(int elem);
    void  swap(int idx1, int idx2);
};




#endif // HEAP_H
