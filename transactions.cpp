#include "transactions.hpp"

Transaction::Transaction(int tradeId, int orderId, int buyerId, int sellerId, string instrument, int quantity, float price) {
    this->tradeId = tradeId;
    this->orderId = orderId;
    this->buyerId = buyerId;
    this->sellerId = sellerId;
    this->instrument = instrument;
    this->quantity = quantity;
    this->price = price;
    this->timestamp = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
}

void Transaction::display() {
    cout << "Trade ID: " << this->tradeId << endl;
    cout << "Order ID: " << this->orderId << endl;
    cout << "Buyer ID: " << this->buyerId << endl;
    cout << "Seller ID: " << this->sellerId << endl;
    cout << "Instrument: " << this->instrument << endl;
    cout << "Quantity: " << this->quantity << endl;
    cout << "Price: " << this->price << endl;
    cout << "Timestamp: " << this->timestamp << endl;
    cout << "----------------------" << endl;
}