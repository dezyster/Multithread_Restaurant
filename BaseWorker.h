#pragma once

#include "Restaurant.h"

class BaseWorker
{
protected:
    Restaurant &m_rest;

    bool checkIfRestaurantIsClosed();

    virtual bool isNeededToWork() = 0;

public:

    BaseWorker(Restaurant &rest): m_rest{ rest }{}

    virtual void work(const char[]) = 0;
};

bool BaseWorker::checkIfRestaurantIsClosed()
{
    std::lock_guard<std::mutex> isRestaurantClosedUniqueLock(m_rest.isRestaurantClosedMutex);
    return m_rest.isRestaurantClosed;
}
