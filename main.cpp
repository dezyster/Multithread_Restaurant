#include "RestaurantFacade.h"

int main()
{
    RestaurantFacade restaurant;

    // setting up restaurant properties if needed
    restaurant.setWorkTime(30); // as default
    restaurant.setMaxOrdersAtOnce(3); // as default
    restaurant.setMinOrderInterval(5); // as default
    restaurant.setMaxOrderInterval(10); // as default

    // ----------- first start -------------

    restaurant.addWaiter("Waiter1");
    restaurant.addWaiter("Waiter2");
    restaurant.addWaiter("Waiter3");
    restaurant.addWaiter("Waiter4");

    restaurant.addChef("Chef1");
    restaurant.addChef("Chef2");

    restaurant.start();
    // ----------- ------------ -------------

    // setting up new restaurant properties for second start
    restaurant.setWorkTime(60);
    restaurant.setMaxOrdersAtOnce(5);
    restaurant.setMinOrderInterval(3);
    restaurant.setMaxOrderInterval(12);

    // ----------- second start -------------
    restaurant.addWaiter("Waiter1");
    restaurant.addWaiter("Waiter2");
    restaurant.addWaiter("Waiter3");
    restaurant.addWaiter("Waiter4");
    restaurant.addWaiter("Waiter5");
    restaurant.addWaiter("Waiter6");

    restaurant.addChef("Chef1");
    restaurant.addChef("Chef2");
    restaurant.addChef("Chef3");

    restaurant.start();
     // ----------- second start -------------

    return 0;
}
