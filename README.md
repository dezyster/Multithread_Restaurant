# Multithread_Restaurant
This program is a simulation of restaurant with parallel-working waiters and chefs

For the restaurant to work, it needs two type of workers: chefs and waiters. The program can work with a different amount of chefs and waiters, at least one waiter and one chef is required. Each worker is a separate thread and can work independently.

To simplify work with restaurant project contains special facade.

A configurable random number of orders are added into the restaurant at random intervals. The task of workers is to process these orders.

The waiters take new orders from customers and take them to the kitchen, after that they notify the chefs. Chefs coock an orders, and after that they notify the waiters. After that, waiters can serve the orders to customer.

For simulation purposes, each action takes some random amount of time.

When the time comes for the restaurant to close, the chefs coock all the remaining orders and go home. The waiters, in turn, wait until all the orders have been served to the customers, and only after that they go home together.

main.cpp file contains examples on how to work with restaurant.
