#pragma once

#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <thread>
#include <string>

#include "Timer.h"

class Restaurant
{
    int ordersToCookCount = 0;
    int ordersToServeCount = 0;
    int newOrdersCount = 0;
    bool isRestaurantClosed = false;

    Timer restaurantClock;

    std::condition_variable waiterWaitingLine;
    std::condition_variable chefWaitingLine;

    std::mutex ordersToServeMutex;
    std::mutex ordersToCookMutex;
    std::mutex newOrdersMutex;
    std::mutex isRestaurantClosedMutex;
    std::mutex coutMutex;

    int tookedNewOrderCount = 0;
    int cookedCount = 0;
    int servedCount = 0;

    bool isNeededToWork();

    std::string getCurrentTime();

public:

    Restaurant(): restaurantClock{}{}

    void workAsChef(const char[]);
    void workAsWaiter(const char[]);

    void addOrders(int);
    void addOrder();

    void closeRestaurant();
};
