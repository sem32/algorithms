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









void countSort(int* A, int length, int range)
{
    int i;
    int j;
    int k;

    int* hist = new int [range];

    for (i = 0; i < range; i++)
    {
        hist[i] = 0;
    }

    for (i = 0; i < length; i++)
    {
        hist[A[i]] = hist[A[i]] + 1;
    }

    k = 0;
    for (i = 0; i < range; i++)
    {
        for(j = hist[i]; j > 0; j--)
        {
            A[k] = i;
            k = k + 1;
        }
    }

    delete [] hist;
}

#define RADIX 10

void countSort2(int* A, int length, int exp)
{
    int i;
    int* hist = new int [RADIX];
    int* out = new int[length];

    for (i = 0; i < RADIX; i++)
    {
        hist[i] = 0;
    }

    for (i = 0; i < length; i++)
    {
        int r = (A[i] / exp) % RADIX;
        hist[r] = hist[r] + 1;
    }

    for (i = 1; i < RADIX; i++)
    {
        hist[i] = hist[i] + hist[i - 1];
    }

    for (i = length - 1; i >= 0; i--)
    {
        int r = (A[i] / exp) % RADIX;
        out[hist[r] - 1] = A[i];
        hist[r] = hist[r] - 1;
    }

    for (i = 0; i < length; i++)
    {
        A[i] = out[i];
    }

    delete [] out;
    delete [] hist;
}

void radixSort(int* A, int length, int range)
{
    int exp;

    for (exp = 1; (range / exp) > 0; exp = exp * RADIX)
    {
        countSort2(A, length, exp);
    }
}
