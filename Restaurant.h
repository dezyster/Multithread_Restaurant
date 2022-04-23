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
public:
    int ordersToCookCount = 0;
    int ordersToServeCount = 0;
    int newOrdersCount = 0;
    bool isRestaurantClosed = false;

    int totalOrders = 0;

    Timer restaurantClock;

    std::condition_variable waiterWaitingLine;
    std::condition_variable chefWaitingLine;

    std::mutex ordersToServeMutex;
    std::mutex ordersToCookMutex;
    std::mutex newOrdersMutex;
    std::mutex isRestaurantClosedMutex;
    std::mutex coutMutex;

    Restaurant(): restaurantClock{}{}

    std::string getCurrentTime();

    void addOrders(int);
    void addOrder();

    void closeRestaurant();
};
