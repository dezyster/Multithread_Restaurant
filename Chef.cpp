#include "Chef.h"

extern int getRandomNumber(int min, int max);

bool ChefWork::isNeededToWork()
{
    if(!(checkIfRestaurantIsClosed()))
    {
        return true;
    }

    std::lock_guard<std::mutex> coutUniqueLock(m_rest.coutMutex);

    return cookedCountNum != m_rest.totalOrders;
}

void ChefWork::goToWaitingLine(std::unique_lock<std::mutex> &ordersToCookUniqueLock)
{
    m_rest.chefWaitingLine.wait(ordersToCookUniqueLock,
        [this]()
        {
            std::lock_guard<std::mutex> isRestaurantClosedUniqueLock(m_rest.isRestaurantClosedMutex);
            return m_rest.ordersToCookCount > 0 || m_rest.isRestaurantClosed;
        });
}

int ChefWork::printStartAndGetOrderNum(const char chefName[])
{
    const auto &currentTime = m_rest.getCurrentTime();

    std::lock_guard<std::mutex> coutUniqueLock(m_rest.coutMutex);
    std::cout << currentTime << ": " << chefName << " started cooking order num. " << ++cookedCountNum << "!" << std::endl;

    return cookedCountNum;
}

void ChefWork::coock()
{
    std::lock_guard<std::mutex> ordersToServeUniqueLock(m_rest.ordersToServeMutex);
    m_rest.ordersToServeCount++;
}

void ChefWork::printFinishCoocking(const char chefName[], int cookingOrderNum)
{
    const auto &currentTime = m_rest.getCurrentTime();

    std::lock_guard<std::mutex> coutUniqueLock(m_rest.coutMutex);
    std::cout << currentTime << ": " << chefName << " cooked an order num. " << cookingOrderNum << "!" << std::endl;
}

void ChefWork::work(const char chefName[])
{
    while (isNeededToWork())
    {
        std::unique_lock<std::mutex> ordersToCookUniqueLock(m_rest.ordersToCookMutex);

        goToWaitingLine(ordersToCookUniqueLock);

        if (m_rest.ordersToCookCount > 0)
        {
            m_rest.ordersToCookCount--;
            ordersToCookUniqueLock.unlock();

            int cookingOrderNum{ printStartAndGetOrderNum(chefName) };

            std::this_thread::sleep_for(std::chrono::milliseconds(getRandomNumber(1000,5000)));

            coock();

            printFinishCoocking(chefName, cookingOrderNum);

            m_rest.waiterWaitingLine.notify_one();

            std::this_thread::sleep_for(std::chrono::milliseconds(getRandomNumber(1000,2000)));
        }
    }
    std::lock_guard<std::mutex> coutUniqueLock(m_rest.coutMutex);
    std::cout << chefName << " goes to home!" << std::endl;
}
