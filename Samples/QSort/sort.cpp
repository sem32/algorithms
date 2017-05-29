#include "stdlib.h"

#define MAX(a, b)   ((a > b) ? a : b)
#define MIN(a, b)   ((a < b) ? a : b)

int median(int a, int b, int c)
{
    int max;
    int min;

    max = MAX(a, MAX(b,c));
    min = MIN(a, MIN(b,c));

    return (a + b + c) - min - max;
}

void swap(int* A, int i, int j)
{
    int t = A[i];
    A[i] = A[j];
    A[j] = t;
}

void partition(int* A, int start, int end)
{
    int left = start;
    int right = end;
    int lambda = A[start + (rand() % (start - end))]; //median(A[(start + end)/2], A[start], A[end]);

    while (left <= right)
    {
        while (A[left] < lambda)
        {
            left = left + 1;
        }

        while (A[right] > lambda)
        {
            right = right - 1;
        }

        if (left <= right)
        {
            swap(A, left, right);
            left = left + 1;
            right = right - 1;
        }
    }

    if (left < end)
    {
        partition(A, left, end);
    }

    if (right > start)
    {
        partition(A, start, right);
    }
}

void qSort(int* A, int length)
{
    partition(A, 0, length - 1);
}
