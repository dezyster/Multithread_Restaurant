#pragma once

#include "Restaurant.h"

class BaseWorker
{
protected:
    Restaurant &m_rest;

    bool checkIfRestaurantIsClosed();

    virtual bool isNeededToWork() = 0;
    virtual void goToWaitingLine(std::unique_lock<std::mutex>&) = 0;

public:

    BaseWorker(Restaurant &rest): m_rest{ rest }{}

    virtual void work(const char[]) = 0;
};
