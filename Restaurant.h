#pragma once

#include <condition_variable>
#include <thread>
#include <string>
#include <list>

#include "Timer.h"
#include "BaseWork.h"

class Restaurant
{
    friend class ChefWork;
    friend class WaiterWork;

    int m_ordersToCookCount = 0;
    int m_ordersToServeCount = 0;
    int m_newOrdersCount = 0;
    bool m_isRestaurantClosed = false;

    int m_totalOrders = 0;

    Timer m_restaurantClock;

    std::condition_variable m_waiterWaitingLine;
    std::condition_variable m_chefWaitingLine;

    std::mutex m_ordersToServeMutex;
    std::mutex m_ordersToCookMutex;
    std::mutex m_newOrdersMutex;
    std::mutex m_isRestaurantClosedMutex;
    std::mutex m_coutAndOtherMutex;

    std::list<std::thread> m_workers;

    void waitUntilWorkEnds();

public:

    Restaurant(): m_restaurantClock{}{}

    void addWorker(const char[], BaseWork&);

    std::string getCurrentTime();

    void addOrders(int);
    void addOrder();

    void resetTimer() { m_restaurantClock.reset(); }
    bool isRestaurantClosed() { return m_isRestaurantClosed; }

    void openRestaurant() { m_isRestaurantClosed = false; }
    void closeRestaurant();

    ~Restaurant();
};
