#include "Restaurant.h"

extern int getRandomNumber(int min, int max);


std::string Restaurant::getCurrentTime()
{
    return std::to_string(restaurantClock.elapsed());
}

void Restaurant::addOrders(int numb)
{
    std::unique_lock<std::mutex> newOrdersUniqueLock(newOrdersMutex);
    newOrdersCount += numb;
    totalOrders += numb;
    newOrdersUniqueLock.unlock();

    waiterWaitingLine.notify_all();
}

void Restaurant::addOrder()
{
    std::unique_lock<std::mutex> newOrdersUniqueLock(newOrdersMutex);
    newOrdersCount ++;
    totalOrders++;
    newOrdersUniqueLock.unlock();

    waiterWaitingLine.notify_one();
}

void Restaurant::closeRestaurant()
{
    {
        std::lock_guard<std::mutex> isRestaurantClosedUniqueLock(isRestaurantClosedMutex);
        isRestaurantClosed = true;
    }
    chefWaitingLine.notify_all();
    waiterWaitingLine.notify_all();
}
