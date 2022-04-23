#pragma once

#include "BaseWorker.h"

class WaiterWork: public BaseWorker
{
    int tookedNewOrderNum = 0;
    int servedCountNum = 0;

    virtual bool isNeededToWork() override;

public:
    WaiterWork(Restaurant &rest):BaseWorker(rest) {}

   virtual void work(const char []) override;
};

extern int getRandomNumber(int min, int max);

bool WaiterWork::isNeededToWork()
{
    {
        std::lock_guard<std::mutex> isRestaurantClosedUniqueLock(m_rest.isRestaurantClosedMutex);
        if(!m_rest.isRestaurantClosed)
        {
            return true;
        }
    }

    std::lock_guard<std::mutex> coutUniqueLock(m_rest.coutMutex);

    return tookedNewOrderNum != servedCountNum;
}

void WaiterWork::work(const char waiterName[])
{
    while (isNeededToWork())
    {
        {
            std::unique_lock<std::mutex> newOrdersUniqueLock(m_rest.newOrdersMutex);
            m_rest.waiterWaitingLine.wait(newOrdersUniqueLock,
                [this]()
                {
                    {
                        std::lock_guard<std::mutex> ordersToServeUniqueLock(m_rest.ordersToServeMutex);
                        if(m_rest.ordersToServeCount > 0)
                        {
                            return true;
                        }
                    }
                    std::lock_guard<std::mutex> isRestaurantClosedUniqueLock(m_rest.isRestaurantClosedMutex);
                    return m_rest.newOrdersCount > 0 || m_rest.isRestaurantClosed;
                });

            if (m_rest.newOrdersCount > 0)
            {
                m_rest.newOrdersCount--;
                newOrdersUniqueLock.unlock();

                std::this_thread::sleep_for(std::chrono::milliseconds(getRandomNumber(1000,2000)));
                std::string currentTime = m_rest.getCurrentTime();

                int newOrderNum;

                {
                    std::lock_guard<std::mutex> coutUniqueLock(m_rest.coutMutex);
                    std::cout << currentTime << ": " << waiterName << " got new order!" << ++tookedNewOrderNum << std::endl;
                    newOrderNum = tookedNewOrderNum;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(getRandomNumber(1000,2000)));

                {
                    std::lock_guard<std::mutex> ordersToCookUniqueLock(m_rest.ordersToCookMutex);
                    m_rest.ordersToCookCount++;
                }

                currentTime = m_rest.getCurrentTime();

                {
                    std::lock_guard<std::mutex> coutUniqueLock(m_rest.coutMutex);
                    std::cout << currentTime << ": " << waiterName << " took order to kitchen!" << newOrderNum << std::endl;
                }

                m_rest.chefWaitingLine.notify_one();

                std::this_thread::sleep_for(std::chrono::milliseconds(getRandomNumber(1000,2000)));
            }
        }

        std::unique_lock<std::mutex> ordersToServeUniqueLock(m_rest.ordersToServeMutex);
        if (m_rest.ordersToServeCount > 0)
        {
            m_rest.ordersToServeCount--;
            ordersToServeUniqueLock.unlock();

            std::this_thread::sleep_for(std::chrono::milliseconds(getRandomNumber(1000,5000)));
            std::string currentTime = m_rest.getCurrentTime();

            std::lock_guard<std::mutex> coutUniqueLock(m_rest.coutMutex);
            std::cout << currentTime << ": " << waiterName << " served an order!" << ++servedCountNum << std::endl;
        }
    }
    std::cout << waiterName << " goes to home!" << std::endl;
}
