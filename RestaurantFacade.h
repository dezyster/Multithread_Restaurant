#pragma once

#include "Restaurant.h"
#include "WaiterWork.h"
#include "ChefWork.h"

class RestaurantFacade
{
    Restaurant m_restaurant;
    WaiterWork m_waiterWork;
    ChefWork m_chefWork;

    int m_workTime = 30; // defines how much time restaurant will work
    int m_maxOrdersAtOnce = 3; //defines maximum amount of orders restaurant can get an one time
    int m_minOrderInterval = 5; //defines minimal interval between orders
    int m_maxOrderInterval = 10; //defines maximum interval between orders

    bool m_wasWaitersAdded = false;
    bool m_wasChefsAdded = false;

public:
    RestaurantFacade()
        : m_restaurant{}, m_waiterWork{ m_restaurant }, m_chefWork{ m_restaurant }{}

    void addWaiter(const char[]);
    void addChef(const char[]);

    void setWorkTime(int workTime) { m_workTime = workTime; }
    void setMaxOrdersAtOnce(int maxOrdersAtOnce) { m_maxOrdersAtOnce = maxOrdersAtOnce; }
    void setMinOrderInterval(int minOrderInterval) { m_minOrderInterval = minOrderInterval; }
    void setMaxOrderInterval(int maxOrderInterval) { m_maxOrderInterval = maxOrderInterval; }

    void start();
};
