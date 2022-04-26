#include <iostream>

#include "Restaurant.h"

extern int getRandomNumber(int min, int max);

void Restaurant::addWorker(const char workerName[], BaseWork &baseWork)
{
    m_workers.push_back( std::thread (&BaseWork::work, std::ref(baseWork), workerName) );
}

std::string Restaurant::getCurrentTime()
{
    return std::to_string(m_restaurantClock.elapsed());
}

void Restaurant::addOrders(int numb)
{
    std::unique_lock<std::mutex> newOrdersUniqueLock(m_newOrdersMutex);

    m_newOrdersCount += numb;
    m_totalOrders += numb;

    newOrdersUniqueLock.unlock();

    m_waiterWaitingLine.notify_all();
}

void Restaurant::waitUntilWorkEnds()
{
    for(auto &it: m_workers)
    {
        it.join();
    }
}

void Restaurant::closeRestaurant()
{
    std::unique_lock<std::mutex> isRestaurantClosedUniqueLock(m_isRestaurantClosedMutex);

    m_isRestaurantClosed = true;

    isRestaurantClosedUniqueLock.unlock();

    m_chefWaitingLine.notify_all();
    m_waiterWaitingLine.notify_all();

    waitUntilWorkEnds();

    std::cout << "Restaurant processed " << m_totalOrders << " orders in " << getCurrentTime() << " seconds" << std::endl;

    m_workers.clear();
    m_totalOrders = 0;
}
