
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "heap.h"

#include <QElapsedTimer>

int FindIndex(int* array, int left, int right, int elem)
{
    int mid = (left + right) / 2;

    if ((right - left) == 1)
    {
        return right;
    }

    if (elem > array[mid])
    {
        return FindIndex(array, mid, right, elem);
    }
    else
    {
        return FindIndex(array, left, mid, elem);
    }
}

int InsertSorted(int* array, int length, int elem)
{
    int idx = FindIndex(array, 0, length - 1, elem);

    for (int i = length; i > idx; i--)
    {
        array[i] = array[i - 1];
    }
    array[idx] = elem;
    return length + 1;
}

int main()
{
    int time1;
    int time2;
    QElapsedTimer  timer;

    int* input1;

    int  length;

    Heap* heap;

    printf("Input length please:\n");

    std::cin>>length;

    printf("\nGenerating input sequence...\n");

    input1 = new int[length + 10];

    heap = new Heap(length);

    for (int i = 0; i < length; i++)
    {
        input1[i] = i;
        heap->add(rand() % length);
    }

    if (length < 10)
    {
        printf("Sorted array:\n");
        for (int i = 0; i < length; i++)
        {
            printf("%d ", input1[i]);
        }
        printf("\nHeap:\n");
        heap->print();
    }

    int elem = length / 2;

    timer.restart();
    length = InsertSorted(input1, length, elem);
    time1 = timer.elapsed();

    timer.restart();
    heap->add(elem);
    time2 = timer.elapsed();

    printf("\nTime = %d ms, %d ms\n", time1, time2);

    printf("\nResults:\n");

    if (length < 10)
    {
        printf("Sorted array:\n");
        for (int i = 0; i < length; i++)
        {
            printf("%d ", input1[i]);
        }
        printf("\nHeap:\n");
        heap->print();
    }

    delete [] input1;
    delete heap;
}
