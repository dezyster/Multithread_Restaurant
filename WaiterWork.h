#pragma once

#include "BaseWork.h"
#include "Restaurant.h"

class WaiterWork: public BaseWork
{
    int m_tookedNewOrderNum = 0;
    int m_servedCountNum = 0;

    virtual bool isNeededToWork() const override;
    virtual void goToWaitingLine(std::unique_lock<std::mutex>&) override;

    int printNewOrderAndGetOrderNum(const char[]);
    void putOrderToKitchen();
    void printTookedOrder(const char[], int);
    void bringOrderToKitchen(const char[]);

    int tookToServe(const char[]);
    void printServed(const char[], int);
    void chechOrdersToServe(const char[]);

    virtual void work(const char []) override;

public:
    WaiterWork(Restaurant &rest): BaseWork(rest){}

    virtual void reset() override;
};
