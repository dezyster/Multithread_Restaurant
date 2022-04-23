#include <conio.h>
#include <random>

#include "Restaurant.h"
#include "Waiter.h"
#include "Chef.h"

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
    srand(static_cast<unsigned int>(time(0)));
    rand();

    resetRandom();

    Restaurant restaurant;

    ChefWork chefWork(restaurant);

    std::thread chef1(&ChefWork::work, std::ref(chefWork), "Chef1");
    std::thread chef2(&ChefWork::work, std::ref(chefWork), "Chef2");

    WaiterWork waiterWork(restaurant);

    std::thread waiter1(&WaiterWork::work, std::ref(waiterWork), "Waiter1");
    std::thread waiter2(&WaiterWork::work, std::ref(waiterWork), "Waiter2");
    std::thread waiter3(&WaiterWork::work, std::ref(waiterWork), "Waiter3");
    std::thread waiter4(&WaiterWork::work, std::ref(waiterWork), "Waiter4");

    Timer workTimer;

    while(workTimer.elapsed() < 30)
    {
        restaurant.addOrders(getRandomNumber(1, 3));
        std::this_thread::sleep_for(std::chrono::seconds(getRandomNumber(3, 10)));
    }

    restaurant.closeRestaurant();

    chef1.join();
    chef2.join();
    waiter1.join();
    waiter2.join();
    waiter3.join();
    waiter4.join();

    return 0;
}
