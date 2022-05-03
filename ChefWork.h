#pragma once

#include "BaseWork.h"
#include "Restaurant.h"

class ChefWork: public BaseWork
{
    int m_cookedCountNum = 0;

    virtual bool isNeededToWork() const override;
    virtual void goToWaitingLine(std::unique_lock<std::mutex>&) override;

    int printStartAndGetOrderNum(const char[]);
    void coock();
    void printFinishCoocking(const char[], int);

    virtual void work(const char[]) override;

public:
    ChefWork(Restaurant &rest): BaseWork(rest){}

    virtual void reset() override;
};
