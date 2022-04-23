#include <conio.h>

#include "Restaurant.h"

extern int getRandomNumber(int min, int max);

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

    std::thread chef1(&Restaurant::workAsChef, std::ref(restaurant), "Chef1");
    std::thread chef2(&Restaurant::workAsChef, std::ref(restaurant), "Chef2");
    std::thread waiter1(&Restaurant::workAsWaiter, std::ref(restaurant), "Waiter1");
    std::thread waiter2(&Restaurant::workAsWaiter, std::ref(restaurant), "Waiter2");
    std::thread waiter3(&Restaurant::workAsWaiter, std::ref(restaurant), "Waiter3");
    std::thread waiter4(&Restaurant::workAsWaiter, std::ref(restaurant), "Waiter4");

    Timer workTimer;

    while(workTimer.elapsed() < 60)
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
