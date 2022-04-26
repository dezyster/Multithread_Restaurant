#include <iostream>

#include "ChefWork.h"

extern int getRandomNumber(int min, int max);

bool ChefWork::isNeededToWork() const
{
    std::unique_lock<std::mutex> isRestaurantClosedUniqueLock(m_rest.m_isRestaurantClosedMutex);
    if(!(m_rest.m_isRestaurantClosed))
    {
        return true;
    }
    isRestaurantClosedUniqueLock.unlock();

    std::lock_guard<std::mutex> coutUniqueLock(m_rest.m_coutAndOtherMutex);

    return m_cookedCountNum != m_rest.m_totalOrders;
}

void ChefWork::goToWaitingLine(std::unique_lock<std::mutex> &ordersToCookUniqueLock)
{
    m_rest.m_chefWaitingLine.wait(ordersToCookUniqueLock,
        [this]()
        {
            std::lock_guard<std::mutex> isRestaurantClosedUniqueLock(m_rest.m_isRestaurantClosedMutex);
            return m_rest.m_ordersToCookCount > 0 || m_rest.m_isRestaurantClosed;
        });
}

int ChefWork::printStartAndGetOrderNum(const char chefName[])
{
    const auto &currentTime = m_rest.getCurrentTime();

    std::lock_guard<std::mutex> coutUniqueLock(m_rest.m_coutAndOtherMutex);
    std::cout << currentTime << ": " << chefName << " started cooking order num. " << ++m_cookedCountNum << "!" << std::endl;

    return m_cookedCountNum;
}

void ChefWork::coock()
{
    std::lock_guard<std::mutex> ordersToServeUniqueLock(m_rest.m_ordersToServeMutex);
    m_rest.m_ordersToServeCount++;
}

void ChefWork::printFinishCoocking(const char chefName[], int cookingOrderNum)
{
    const auto &currentTime = m_rest.getCurrentTime();

    std::lock_guard<std::mutex> coutUniqueLock(m_rest.m_coutAndOtherMutex);
    std::cout << currentTime << ": " << chefName << " cooked an order num. " << cookingOrderNum << "!" << std::endl;
}

void ChefWork::work(const char chefName[])
{
    while (isNeededToWork())
    {
        std::unique_lock<std::mutex> ordersToCookUniqueLock(m_rest.m_ordersToCookMutex);

        goToWaitingLine(ordersToCookUniqueLock);

        if (m_rest.m_ordersToCookCount > 0)
        {
            m_rest.m_ordersToCookCount--;
            ordersToCookUniqueLock.unlock();

            int cookingOrderNum{ printStartAndGetOrderNum(chefName) };

            std::this_thread::sleep_for(std::chrono::milliseconds(getRandomNumber(1000,5000)));

            coock();

            printFinishCoocking(chefName, cookingOrderNum);

            m_rest.m_waiterWaitingLine.notify_one();

            std::this_thread::sleep_for(std::chrono::milliseconds(getRandomNumber(1000,2000)));
        }
    }
    std::lock_guard<std::mutex> coutUniqueLock(m_rest.m_coutAndOtherMutex);
    std::cout << chefName << " goes to home!" << std::endl;
}

void ChefWork::reset()
{
    std::lock_guard<std::mutex> coutUniqueLock(m_rest.m_coutAndOtherMutex);
    m_cookedCountNum = 0;
}
