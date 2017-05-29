#include <memory.h>
#include <stdio.h>
#include "heap.h"

Heap::Heap(int size)
{
    heapArray = new int[size + 10];
    memset(heapArray, 0, size*sizeof(int));
    length = size;
    currentSize = 0;
}

Heap::~Heap()
{
    delete [] heapArray;
}

void Heap::siftUp(int index)
{
    if (index == 0)
    {
        return;
    }

    int parentIndex = (index / 2);

    if (heapArray[parentIndex] > heapArray[index])
    {
        swap(parentIndex, index);
        siftUp(parentIndex);
    }
}

void Heap::siftDown(int index)
{
    int child1Index = (index * 2) + 1;
    int child2Index = (index * 2) + 2;
    int childIndex = 0;

    if (child2Index >= currentSize / 2)
    {
        return;
    }

    if (heapArray[child1Index] < heapArray[child2Index])
    {
        childIndex = child1Index;
    }
    else
    {
        childIndex = child2Index;
    }

    if (heapArray[childIndex] < heapArray[index])
    {
        swap(childIndex, index);
        siftDown(childIndex);
    }
}

void Heap::swap(int idx1, int idx2)
{
    int t = heapArray[idx1];
    heapArray[idx1] = heapArray[idx2];
    heapArray[idx2] = t;
}

void Heap::add(int elem)
{
    heapArray[currentSize] = elem;
    siftUp(currentSize);
    currentSize = currentSize + 1;
}

void Heap::print()
{
    for (int i = 0; i < currentSize; i++)
    {
        printf("%d ", heapArray[i]);
    }
}
