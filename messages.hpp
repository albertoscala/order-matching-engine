#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include<iostream>
#include<optional>
#include<sstream>
#include<vector>
using namespace std;

vector<string> split(string s, char delimiter);

enum class Side {
    BUY = 1,
    SELL = 2
};

enum class OrderType {
    MARKET = 1,
    LIMIT = 2
};

class Message {
private:
    int             orderId;            // Unique identifier for the order
    int             clientId;           // Client placing the order
    string          instrument;         // Financial instrument
    Side            side;               // Buy or sell
    int             quantity;           // Number of units to trade
    OrderType       type;               // Order type
    optional<int>   price;              // Price for the limit or stop-limit orders
public:
    Message(int orderId, int clientId, string instrument, Side side, int quantity, OrderType type, optional<int> price);

    Message(string fixMessage);

    void display();

    string toFixMessage();

    // Getters
    int getOrderId();

    int getClientId();

    string getInstrument();

    int getSide();

    int getQuantity();

    int getType();

    optional<int> getPrice();
};

class MessageBuilder {
private:
    int             orderId;            // Unique identifier for the order
    int             clientId;           // Client placing the order
    string          instrument;         // Financial instrument
    Side            side;               // Buy or sell
    int             quantity;           // Number of units to trade
    OrderType       type;               // Order type
    optional<int>   price;              // Price for the limit or stop-limit orders
public:
    MessageBuilder(int orderId, int clientId, string instrument, Side side, int quantity, OrderType type);

    MessageBuilder& setPrice(int price);

    Message build();
};

#endif