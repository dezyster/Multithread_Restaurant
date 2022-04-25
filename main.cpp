#include <conio.h>
#include <random>
#include <iostream>

#include "Restaurant.h"
#include "Waiter.h"
#include "Chef.h"

#define WORK_TIME 30 //defines how many seconds restaurant will work

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

int main()
{
    resetRandom();

    Restaurant restaurant;

    WaiterWork waiterWork(restaurant);

    waiterWork.addWaiterToRestaurant("Waiter1");
    waiterWork.addWaiterToRestaurant("Waiter2");
    waiterWork.addWaiterToRestaurant("Waiter3");
    waiterWork.addWaiterToRestaurant("Waiter4");

    ChefWork chefWork(restaurant);

    chefWork.addChefToRestaurant("Chef1");
    chefWork.addChefToRestaurant("Chef2");

    Timer workTimer;

    while(workTimer.elapsed() < WORK_TIME)
    {
        restaurant.addOrders(getRandomNumber(1, 3));
        std::this_thread::sleep_for(std::chrono::seconds(getRandomNumber(5, 10)));
    }

    restaurant.closeRestaurant();

    std::cout << "Restaurant processed " << restaurant.getTotalOrders() << " orders in "
              << restaurant.getCurrentTime() << " seconds";

    return 0;
}
