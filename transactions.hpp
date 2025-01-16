#ifndef TRANSACTIONS_HPP
#define TRANSACTIONS_HPP

#include<iostream>
#include<chrono>
using namespace std;

class Transaction {
private:
    int             tradeId;            // Unique identifier for the order
    int             orderId;            // Client placing the order
    int             buyerId;            // Client placing the order
    int             sellerId;           // Client placing the order
    string          instrument;         // Financial instrument
    int             quantity;           // Number of units to trade
    float           price;              // Price for the limit or stop-limit orders
    long long       timestamp;          // Time of the transaction
public:
    Transaction(int tradeId, int orderId, int buyerId, int sellerId, string instrument, int quantity, float price);

    void display();

    string inlineDisplay();
};

#endif