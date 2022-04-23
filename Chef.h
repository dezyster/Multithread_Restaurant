#pragma once

#include "BaseWorker.h"

class ChefWork: public BaseWorker
{
    int cookedCountNum = 0;

    virtual bool isNeededToWork() override;

    virtual void goToWaitingLine(std::unique_lock<std::mutex>&) override;

    int printStartAndGetOrderNum(const char[]);
    void coock();
    void printFinishCoocking(const char[], int);

public:
    ChefWork(Restaurant &rest):BaseWorker(rest) {}

    virtual void work(const char[]) override;
};
