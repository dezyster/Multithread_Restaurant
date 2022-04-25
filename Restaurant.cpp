#include "Restaurant.h"

extern int getRandomNumber(int min, int max);

void Restaurant::addWorker(const char workerName[], BaseWork &baseWork)
{
    workers.push_back( std::thread (&BaseWork::work, std::ref(baseWork), workerName) );
}

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

void Restaurant::waitUntilWorkEnds()
{
    for(auto &it: workers)
    {
        it.join();
    }
}

void Restaurant::closeRestaurant()
{
    {
        std::lock_guard<std::mutex> isRestaurantClosedUniqueLock(isRestaurantClosedMutex);
        isRestaurantClosed = true;
    }
    chefWaitingLine.notify_all();
    waiterWaitingLine.notify_all();

    waitUntilWorkEnds();
    workers.clear();
}
