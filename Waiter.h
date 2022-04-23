#pragma once

#include "BaseWorker.h"

class WaiterWork: public BaseWorker
{
    int tookedNewOrderNum = 0;
    int servedCountNum = 0;

    virtual bool isNeededToWork() override;

    virtual void goToWaitingLine(std::unique_lock<std::mutex>&) override;

    int printNewOrderAndGetOrderNum(const char[]);
    void takeOrderToKitchen();
    void printTookedOrder(const char[], int);
    void printServed(const char[]);
    bool checkNewOrders(const char[]);

public:
    WaiterWork(Restaurant &rest):BaseWorker(rest) {}

    virtual void work(const char []) override;
};
