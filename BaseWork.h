#pragma once

#include <mutex>

class Restaurant;

class BaseWork
{
protected:
    Restaurant &m_rest;

    virtual bool isNeededToWork() const = 0;
    virtual void goToWaitingLine(std::unique_lock<std::mutex>&) = 0;

public:
    BaseWork(Restaurant &rest): m_rest{ rest }{}

    virtual void work(const char[]) = 0;
    virtual void reset() = 0;
};
