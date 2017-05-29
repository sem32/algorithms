
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <QElapsedTimer>

void mergeSort(int* A, int start, int end);
void mergeSortBU(int* A, int length);

int main()
{
    QElapsedTimer  timer;

    int i;

    int time1 = 0;
    int time2 = 0;
    int time3 = 0;

    int* inputRandom;
    int* inputSorted;
    int* inputBacksorted;

    int  length;

    printf("Input length please:\n");

    std::cin>>length;

    printf("\nGenerating input sequence...\n");

    inputRandom = new int[length];
    inputSorted = new int[length];
    inputBacksorted = new int[length];

    for (int i = 0; i < length; i++)
    {
        inputRandom[i] = rand() % length;
        inputSorted[i] = i;
        inputBacksorted[i] = length - i;

        if (length < 20)
        {
            printf("%d\t%d\t%d\n", inputRandom[i], inputSorted[i], inputBacksorted[i]);
        }
    }

    printf("\nPerforming merge sort\n");

    timer.restart();
    mergeSortBU(inputRandom, length);
    //mergeSort(inputRandom, 0, length - 1);
    time1 = timer.elapsed();

    timer.restart();
    mergeSortBU(inputSorted, length);
    //mergeSort(inputSorted, 0, length - 1);
    time2 = timer.elapsed();

    timer.restart();
    mergeSortBU(inputBacksorted, length);
    //mergeSort(inputBacksorted, 0, length - 1);
    time3 = timer.elapsed();

    printf("\nTime = %d ms, %d ms, %d ms\n", time1, time2, time3);

    printf("\nResults:\n");

    for (i = 0; i < length; i++)
    {
        if (length < 20)
        {
            printf("%d\t%d\t%d\n", inputRandom[i], inputSorted[i], inputBacksorted[i]);
        }

        if (i < (length -1))
        {
            if ((inputRandom[i + 1] < inputRandom[i]) ||
                (inputSorted[i + 1] < inputSorted[i]) ||
                (inputBacksorted[i + 1] < inputBacksorted[i])
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

    delete [] inputRandom;
    delete [] inputSorted;
    delete [] inputBacksorted;
}
