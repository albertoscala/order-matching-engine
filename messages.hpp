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
    LIMIT = 2,
    STOP = 3,
    STOP_LIMIT = 4
};

class Message {
private:
    optional<int> orderId;          // Unique identifier for the order
    optional<int> clientId;         // Client placing the order
    optional<string> instrument;    // Financial instrument
    optional<int> side;             // Buy or sell
    optional<int> quantity;         // Number of units to trade
    optional<int> type;             // Order type
    optional<int> price;            // Price for the limit or stop-limit orders
    optional<int> stopPrice;        // Stop price for the stop and stop-limit orders
public:
    Message(optional<int> orderId, optional<int> clientId, optional<string> instrument, optional<int> side, optional<int> quantity, optional<int> type, optional<int> price, optional<int> stopPrice);

    Message(string fixMessage);

    void display();

    string toFixMessage();
};

class MessageBuilder {
private:
    optional<int> orderId;      // Unique identifier for the order
    optional<int> clientId;     // Client placing the order
    optional<string> instrument;   // Financial instrument
    optional<int> side;         // Buy or sell
    optional<int> quantity;     // NUmber of units to trade
    optional<int> type;         // Order type
    optional<int> price;        // Price for the limit or stop-limit orders
    optional<int> stopPrice;    // Stop price for the stop and stop-limit orders
public:
    MessageBuilder& setOrderId(int orderId);

    MessageBuilder& setClientId(int clientId);

    MessageBuilder& setInstrument(string instrument);

    MessageBuilder& setSide(Side side);

    MessageBuilder& setQuantity(int quantity);

    MessageBuilder& setType(OrderType type);

    MessageBuilder& setPrice(int price);

    MessageBuilder& setStopPrice(int stopPrice);

    Message build();
};

#endif