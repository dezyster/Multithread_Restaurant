#pragma once

#include "BaseWorker.h"

class WaiterWork: public BaseWorker
{
    int tookedNewOrderNum = 0;
    int servedCountNum = 0;

    virtual bool isNeededToWork() override;

    bool checkNewOrders(const char[]);
    int printNewOrderAndGetOrderNum(const char[]);
    void takeOrderToKitchen();
    void printTookedOrder(const char[], int);

public:
    WaiterWork(Restaurant &rest):BaseWorker(rest) {}

    virtual void work(const char []) override;
};

extern int getRandomNumber(int min, int max);

bool WaiterWork::isNeededToWork()
{
    if(!(checkIfRestaurantIsClosed()))
    {
        return true;
    }

    std::lock_guard<std::mutex> coutUniqueLock(m_rest.coutMutex);

    return tookedNewOrderNum != servedCountNum;
}


int WaiterWork::printNewOrderAndGetOrderNum(const char waiterName[])
{
    const auto &currentTime = m_rest.getCurrentTime();

    std::lock_guard<std::mutex> coutUniqueLock(m_rest.coutMutex);

    std::cout << currentTime << ": " << waiterName << " got new order num. " << ++tookedNewOrderNum << "!" << std::endl;

    return tookedNewOrderNum;
}

void WaiterWork::takeOrderToKitchen()
{
    std::lock_guard<std::mutex> ordersToCookUniqueLock(m_rest.ordersToCookMutex);
    m_rest.ordersToCookCount++;
}

void WaiterWork::printTookedOrder(const char waiterName[], int newOrderNum)
{
    const auto &currentTime = m_rest.getCurrentTime();
    std::lock_guard<std::mutex> coutUniqueLock(m_rest.coutMutex);
    std::cout << currentTime << ": " << waiterName << " took order num. " << newOrderNum << " to kitchen!" << std::endl;
}

bool WaiterWork::checkNewOrders(const char waiterName[])
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

        int newOrderNum{ printNewOrderAndGetOrderNum(waiterName) };

        std::this_thread::sleep_for(std::chrono::milliseconds(getRandomNumber(1000,2000)));

        takeOrderToKitchen();

        printTookedOrder(waiterName, newOrderNum);

        m_rest.chefWaitingLine.notify_one();

        std::this_thread::sleep_for(std::chrono::milliseconds(getRandomNumber(1000,2000)));
    }
}

void WaiterWork::work(const char waiterName[])
{
    while (isNeededToWork())
    {
        checkNewOrders(waiterName);

        std::unique_lock<std::mutex> ordersToServeUniqueLock(m_rest.ordersToServeMutex);
        if (m_rest.ordersToServeCount > 0)
        {
            m_rest.ordersToServeCount--;
            ordersToServeUniqueLock.unlock();

            std::this_thread::sleep_for(std::chrono::milliseconds(getRandomNumber(1000,5000)));
            std::string currentTime = m_rest.getCurrentTime();

            std::lock_guard<std::mutex> coutUniqueLock(m_rest.coutMutex);
            std::cout << currentTime << ": " << waiterName << " served an order num. " << ++servedCountNum << "!" << std::endl;
        }
    }
    std::cout << waiterName << " goes to home!" << std::endl;
}
