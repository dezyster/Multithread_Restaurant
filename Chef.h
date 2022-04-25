#pragma once

#include "BaseWorker.h"
#include "Restaurant.h"

class ChefWork: public BaseWork
{
    int cookedCountNum = 0;

    virtual bool isNeededToWork() override;

    virtual void goToWaitingLine(std::unique_lock<std::mutex>&) override;

    int printStartAndGetOrderNum(const char[]);
    void coock();
    void printFinishCoocking(const char[], int);

    virtual void work(const char[]) override;

public:
    ChefWork(Restaurant &rest):BaseWork(rest) {}

    void addChefToRestaurant(const char chefName[]) { m_rest.addWorker(chefName, *this); }
};
