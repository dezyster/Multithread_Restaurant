#pragma once

#include <condition_variable>
#include <thread>
#include <string>
#include <list>

#include "Timer.h"
#include "BaseWorker.h"

class Restaurant
{
    friend class ChefWork;
    friend class WaiterWork;

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

    std::list<std::thread> workers;

    void addWorker(const char[], BaseWork&);

    void waitUntilWorkEnds();

public:

    Restaurant(): restaurantClock{}{}

    std::string getCurrentTime();

    void addOrders(int);
    void addOrder();

    int getTotalOrders(){ return totalOrders; }

    void closeRestaurant();

    ~Restaurant() { waitUntilWorkEnds(); }
};
