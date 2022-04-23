#include <random>

#include "Restaurant.h"

int getRandomNumber(int min, int max)
{
    static std::mt19937 generator;
    std::uniform_int_distribution<int> distribution(min,max);
    return distribution(generator);
}

bool Restaurant::isNeededToWork()
{
    {
        std::lock_guard<std::mutex> newOrdersUniqueLock(newOrdersMutex);
        if(newOrdersCount > 0)
        {
            return true;
        }
    }
    {
        std::lock_guard<std::mutex> ordersToServeUniqueLock(ordersToServeMutex);
        if(ordersToServeCount > 0)
        {
            return true;
        }
    }
    {
        std::lock_guard<std::mutex> ordersToCookUniqueLock(ordersToCookMutex);
        if(ordersToCookCount > 0)
        {
            return true;
        }
    }

    std::lock_guard<std::mutex> isRestaurantClosedUniqueLock(isRestaurantClosedMutex);
    return !isRestaurantClosed;
}

std::string Restaurant::getCurrentTime()
{
    return std::to_string(restaurantClock.elapsed());
}

void Restaurant::workAsChef(const char chefName[])
{
    while (isNeededToWork())
    {
        std::unique_lock<std::mutex> ordersToCookUniqueLock(ordersToCookMutex);
        chefWaitingLine.wait(ordersToCookUniqueLock,
            [this]()
            {
                std::lock_guard<std::mutex> isRestaurantClosedUniqueLock(isRestaurantClosedMutex);
                return ordersToCookCount > 0 || isRestaurantClosed;
            });

        if (ordersToCookCount > 0)
        {
            ordersToCookCount--;
            ordersToCookUniqueLock.unlock();

            std::this_thread::sleep_for(std::chrono::milliseconds(getRandomNumber(1000,5000)));
            std::string currentTime = getCurrentTime();

            int cookingOrderNum;

            {
                std::lock_guard<std::mutex> coutUniqueLock(coutMutex);
                std::cout << currentTime << ": " << chefName << " started cooking order! " << ++cookedCount << std::endl;
                cookingOrderNum = cookedCount;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(getRandomNumber(1000,5000)));

            {
                std::lock_guard<std::mutex> ordersToServeUniqueLock(ordersToServeMutex);
                ordersToServeCount++;
            }

            currentTime = getCurrentTime();

            {
                std::lock_guard<std::mutex> coutUniqueLock(coutMutex);
                std::cout << currentTime << ": " << chefName << " cooked an order! " << cookingOrderNum << std::endl;
            }

            waiterWaitingLine.notify_one();

            std::this_thread::sleep_for(std::chrono::milliseconds(getRandomNumber(1000,2000)));
        }
    }
    std::cout << chefName << " goes to home!" << std::endl;
}

void Restaurant::workAsWaiter(const char waiterName[])
{
    while (isNeededToWork())
    {
        {
            std::unique_lock<std::mutex> newOrdersUniqueLock(newOrdersMutex);
            waiterWaitingLine.wait(newOrdersUniqueLock,
                [this]()
                {
                    {
                        std::lock_guard<std::mutex> ordersToServeUniqueLock(ordersToServeMutex);
                        if(ordersToServeCount > 0)
                        {
                            return true;
                        }
                    }
                    std::lock_guard<std::mutex> isRestaurantClosedUniqueLock(isRestaurantClosedMutex);
                    return newOrdersCount > 0 || isRestaurantClosed;
                });

            if (newOrdersCount > 0)
            {
                newOrdersCount--;
                newOrdersUniqueLock.unlock();

                std::this_thread::sleep_for(std::chrono::milliseconds(getRandomNumber(1000,2000)));
                std::string currentTime = getCurrentTime();

                int newOrderNum;

                {
                    std::lock_guard<std::mutex> coutUniqueLock(coutMutex);
                    std::cout << currentTime << ": " << waiterName << " got new order!" << ++tookedNewOrderCount << std::endl;
                    newOrderNum = tookedNewOrderCount;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(getRandomNumber(1000,2000)));

                {
                    std::lock_guard<std::mutex> ordersToCookUniqueLock(ordersToCookMutex);
                    ordersToCookCount++;
                }

                currentTime = getCurrentTime();

                {
                    std::lock_guard<std::mutex> coutUniqueLock(coutMutex);
                    std::cout << currentTime << ": " << waiterName << " took order to kitchen!" << newOrderNum << std::endl;
                }

                chefWaitingLine.notify_one();

                std::this_thread::sleep_for(std::chrono::milliseconds(getRandomNumber(1000,2000)));
            }
        }
        std::unique_lock<std::mutex> ordersToServeUniqueLock(ordersToServeMutex);
        if (ordersToServeCount > 0)
        {
            ordersToServeCount--;
            ordersToServeUniqueLock.unlock();

            std::this_thread::sleep_for(std::chrono::milliseconds(getRandomNumber(1000,5000)));
            std::string currentTime = getCurrentTime();

            std::lock_guard<std::mutex> coutUniqueLock(coutMutex);
            std::cout << currentTime << ": " << waiterName << " served an order!" << ++servedCount << std::endl;
        }
    }
    std::cout << waiterName << " goes to home!" << std::endl;
}

void Restaurant::addOrders(int numb)
{
    std::unique_lock<std::mutex> newOrdersUniqueLock(newOrdersMutex);
    newOrdersCount += numb;
    newOrdersUniqueLock.unlock();

    waiterWaitingLine.notify_all();
}

void Restaurant::addOrder()
{
    std::unique_lock<std::mutex> newOrdersUniqueLock(newOrdersMutex);
    newOrdersCount ++;
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
