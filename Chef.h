#pragma once

#include "BaseWorker.h"

class ChefWork: public BaseWorker
{
    int cookedCountNum = 0;

    virtual bool isNeededToWork() override;

public:
    ChefWork(Restaurant &rest):BaseWorker(rest) {}

    virtual void work(const char[]) override;
};

extern int getRandomNumber(int min, int max);

bool ChefWork::isNeededToWork()
{
    {
        std::lock_guard<std::mutex> isRestaurantClosedUniqueLock(m_rest.isRestaurantClosedMutex);
        if(!m_rest.isRestaurantClosed)
        {
            return true;
        }
    }

    std::lock_guard<std::mutex> coutUniqueLock(m_rest.coutMutex);

    return cookedCountNum != m_rest.totalOrders;
}

void ChefWork::work(const char chefName[])
{
    while (isNeededToWork())
    {
        std::unique_lock<std::mutex> ordersToCookUniqueLock(m_rest.ordersToCookMutex);
        m_rest.chefWaitingLine.wait(ordersToCookUniqueLock,
            [this]()
            {
                std::lock_guard<std::mutex> isRestaurantClosedUniqueLock(m_rest.isRestaurantClosedMutex);
                return m_rest.ordersToCookCount > 0 || m_rest.isRestaurantClosed;
            });

        if (m_rest.ordersToCookCount > 0)
        {
            m_rest.ordersToCookCount--;
            ordersToCookUniqueLock.unlock();

            std::this_thread::sleep_for(std::chrono::milliseconds(getRandomNumber(1000,5000)));
            std::string currentTime = m_rest.getCurrentTime();

            int cookingOrderNum;

            {
                std::lock_guard<std::mutex> coutUniqueLock(m_rest.coutMutex);
                std::cout << currentTime << ": " << chefName << " started cooking order! " << ++cookedCountNum << std::endl;
                cookingOrderNum = cookedCountNum;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(getRandomNumber(1000,5000)));

            {
                std::lock_guard<std::mutex> ordersToServeUniqueLock(m_rest.ordersToServeMutex);
                m_rest.ordersToServeCount++;
            }

            currentTime = m_rest.getCurrentTime();

            {
                std::lock_guard<std::mutex> coutUniqueLock(m_rest.coutMutex);
                std::cout << currentTime << ": " << chefName << " cooked an order! " << cookingOrderNum << std::endl;
            }

            m_rest.waiterWaitingLine.notify_one();

            std::this_thread::sleep_for(std::chrono::milliseconds(getRandomNumber(1000,2000)));
        }
    }
    std::cout << chefName << " goes to home!" << std::endl;
}
