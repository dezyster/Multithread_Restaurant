#include <random>
#include <conio.h>
#include <iostream>

#include "Timer.h"

int getRandomNumber(int min, int max)
{
    static std::mt19937 generator;
    std::uniform_int_distribution<int> distribution(min,max);
    return distribution(generator);
}

void resetRandom()
{
    Timer randomTimer;

    std::cout << "Press any key to start" << std::endl;
    getch();

    for(int i = 0; i < ( randomTimer.elapsed() * 10000); i++)
    {
        getRandomNumber(1,10);
    }
}
