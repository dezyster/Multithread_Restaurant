#pragma once

#include "BaseWorker.h"
#include "Restaurant.h"

class WaiterWork: public BaseWork
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

    virtual void work(const char []) override;

public:
    WaiterWork(Restaurant &rest):BaseWork(rest) {}

    void addWaiterToRestaurant(const char waiterName[]) { m_rest.addWorker(waiterName, *this); }
};
