#include <iostream>

#include "WaiterWork.h"

extern int getRandomNumber(int min, int max);

bool WaiterWork::isNeededToWork() const
{
    std::unique_lock<std::mutex> isRestaurantClosedUniqueLock(m_rest.m_isRestaurantClosedMutex);
    if(!(m_rest.m_isRestaurantClosed))
    {
        return true;
    }
    isRestaurantClosedUniqueLock.unlock();

    std::lock_guard<std::mutex> coutAndOtherUniqueLock(m_rest.m_coutAndOtherMutex);
    return m_tookedNewOrderNum != m_servedCountNum;
}

void WaiterWork::goToWaitingLine(std::unique_lock<std::mutex> &newOrdersUniqueLock)
{
    m_rest.m_waiterWaitingLine.wait(newOrdersUniqueLock,
        [this]()
        {
            std::unique_lock<std::mutex> ordersToServeUniqueLock(m_rest.m_ordersToServeMutex);
            if(m_rest.m_ordersToServeCount > 0)
            {
                return true;
            }
            ordersToServeUniqueLock.unlock();

            std::lock_guard<std::mutex> isRestaurantClosedUniqueLock(m_rest.m_isRestaurantClosedMutex);
            return m_rest.m_newOrdersCount > 0 || m_rest.m_isRestaurantClosed;
        });
}

int WaiterWork::printNewOrderAndGetOrderNum(const char waiterName[])
{
    const auto &currentTime = m_rest.getCurrentTime();

    std::lock_guard<std::mutex> coutUniqueLock(m_rest.m_coutAndOtherMutex);

    std::cout << currentTime << ": " << waiterName << " got new order num. " << ++m_tookedNewOrderNum << "!" << std::endl;

    return m_tookedNewOrderNum;
}

void WaiterWork::takeOrderToKitchen()
{
    std::lock_guard<std::mutex> ordersToCookUniqueLock(m_rest.m_ordersToCookMutex);
    m_rest.m_ordersToCookCount++;
}

void WaiterWork::printTookedOrder(const char waiterName[], int newOrderNum)
{
    const auto &currentTime = m_rest.getCurrentTime();
    std::lock_guard<std::mutex> coutUniqueLock(m_rest.m_coutAndOtherMutex);
    std::cout << currentTime << ": " << waiterName << " took order num. " << newOrderNum << " to kitchen!" << std::endl;
}

void WaiterWork::printServed(const char waiterName[])
{
   const auto &currentTime{ m_rest.getCurrentTime() };

    std::lock_guard<std::mutex> coutUniqueLock(m_rest.m_coutAndOtherMutex);
    std::cout << currentTime << ": " << waiterName << " served an order num. " << ++m_servedCountNum << "!" << std::endl;
}

bool WaiterWork::checkNewOrders(const char waiterName[])
{
    std::unique_lock<std::mutex> newOrdersUniqueLock(m_rest.m_newOrdersMutex);

    goToWaitingLine(newOrdersUniqueLock);

    if (m_rest.m_newOrdersCount > 0)
    {
        m_rest.m_newOrdersCount--;
        newOrdersUniqueLock.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(getRandomNumber(1000,2000)));

        int newOrderNum{ printNewOrderAndGetOrderNum(waiterName) };

        std::this_thread::sleep_for(std::chrono::milliseconds(getRandomNumber(1000,2000)));

        takeOrderToKitchen();

        printTookedOrder(waiterName, newOrderNum);

        m_rest.m_chefWaitingLine.notify_one();

        std::this_thread::sleep_for(std::chrono::milliseconds(getRandomNumber(1000,2000)));
    }
}

void WaiterWork::work(const char waiterName[])
{
    while (isNeededToWork())
    {
        checkNewOrders(waiterName);

        std::unique_lock<std::mutex> ordersToServeUniqueLock(m_rest.m_ordersToServeMutex);
        if (m_rest.m_ordersToServeCount > 0)
        {
            m_rest.m_ordersToServeCount--;
            ordersToServeUniqueLock.unlock();

            std::this_thread::sleep_for(std::chrono::milliseconds(getRandomNumber(1000,5000)));

            printServed(waiterName);
        }
    }
    std::lock_guard<std::mutex> coutUniqueLock(m_rest.m_coutAndOtherMutex);
    std::cout << waiterName << " goes to home!" << std::endl;
}

void WaiterWork::reset()
{
    std::lock_guard<std::mutex> coutAndOtherUniqueLock(m_rest.m_coutAndOtherMutex);
    m_tookedNewOrderNum = 0;
    m_servedCountNum = 0;
}
