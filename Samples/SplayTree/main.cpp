
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <QElapsedTimer>

#include "splay.h"

int main()
{
    int time1;
    int time2;
    QElapsedTimer  timer;

    int  length;

    SplayTree* splay = new SplayTree();

    printf("Input length please:\n");

    std::cin>>length;

    printf("\nBuilding splay tree...\n");

    for (int i = 0; i < length; i++)
    {
        splay->add(i);
    }

    // Search random values
    timer.restart();
    for (int i = 0; i < length; i++)
    {
        splay->get(rand() % length);
    }
    time1 = timer.elapsed();

    timer.restart();
    // Search random values in certain range
    const int RANGE = length / 10;

    for (int i = 0; i < length; i++)
    {
        splay->get(rand() % RANGE);
    }
    time2 = timer.elapsed();

    printf("\nTime = %d ms, %d ms\n\n", time1, time2);

    // Check if it is OK
    //splay->check();

    printf("\n");

    delete splay;
}
