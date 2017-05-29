
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <QElapsedTimer>

#define MAX 10000000

void qSort(int* A, int length);

void countSort(int* A, int length, int range);
void radixSort(int* A, int length, int range);

int main()
{
    QElapsedTimer  timer;

    int i;

    int time1 = 0;
    int time2 = 0;
    int time3 = 0;

    int* input1;
    int* input2;
    int* input3;

    int  length;

    printf("Input length please:\n");

    std::cin>>length;

    printf("\nGenerating input sequence...\n");

    input1 = new int[length];
    input2 = new int[length];
    input3 = new int[length];

    for (int i = 0; i < length; i++)
    {
        input1[i] = rand() % MAX;
        input2[i] = input1[i];
        input3[i] = input1[i];

        if (length < 20)
        {
            printf("%d\t%d\t%d\n", input1[i], input2[i], input3[i]);
        }
    }

    printf("\nPerforming quick sort\n");

    timer.restart();
    qSort(input1, length);
    time1 = timer.elapsed();

    timer.restart();
    countSort(input2, length, MAX);
    time2 = timer.elapsed();

    timer.restart();
    radixSort(input3, length, MAX);
    time3 = timer.elapsed();

    printf("\nTime = %d ms, %d ms, %d ms\n", time1, time2, time3);

    printf("\nResults:\n");

    for (i = 0; i < length; i++)
    {
        if (length < 20)
        {
            printf("%d\t%d\t%d\n", input1[i], input2[i], input3[i]);
        }

        if (i < (length -1))
        {
            if ((input1[i + 1] < input1[i]) ||
                (input2[i + 1] < input2[i]) ||
                (input3[i + 1] < input3[i])
               )
            {
                break;
            }
        }
    }

    if (i == length)
    {
        printf("OK...\n");
    }
    else
    {
        printf("Wrong...\n");
    }

    delete [] input1;
    delete [] input2;
    delete [] input3;
}
