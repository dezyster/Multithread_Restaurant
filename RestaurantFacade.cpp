#include <iostream>

#include "RestaurantFacade.h"

extern int getRandomNumber(int min, int max);
extern void resetRandom();

void RestaurantFacade::addWaiter(const char waiterName[])
{
    m_wasWaitersAdded = true;
    m_restaurant.addWorker(waiterName, m_waiterWork);
}

void RestaurantFacade::addChef(const char chefName[])
{
    m_wasChefsAdded = true;
    m_restaurant.addWorker(chefName, m_chefWork);
}

void RestaurantFacade::start()
{
    if(m_wasChefsAdded && m_wasWaitersAdded)
    {
        resetRandom();

        m_restaurant.resetTimer();

        Timer workTimer;

        while(workTimer.elapsed() < m_workTime)
        {
            m_restaurant.addOrders(getRandomNumber(1, m_maxOrdersAtOnce));
            std::this_thread::sleep_for(std::chrono::seconds(getRandomNumber(m_minOrderInterval, m_maxOrderInterval)));
        }

        m_restaurant.closeRestaurant();

        m_waiterWork.reset();
        m_chefWork.reset();
        m_wasWaitersAdded = false;
        m_wasChefsAdded = false;

        m_restaurant.openRestaurant();
    }
    else
    {
        std::cout << "You need to add waiters and chefs for restaurant to work!" << std::endl;
    }
}
