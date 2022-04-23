#pragma once

#include "Restaurant.h"

class BaseWorker
{
protected:
    Restaurant &m_rest;

    virtual bool isNeededToWork() = 0;

public:

    BaseWorker(Restaurant &rest): m_rest{ rest }{}

    virtual void work(const char[]) = 0;
};
