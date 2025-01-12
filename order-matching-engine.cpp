#include<iostream>
#include<optional>
#include<sstream>
#include<vector>
#include<map>
#include"messages.hpp"
using namespace std;

// Custom comparator for reverse sorting
struct ReverseSort {
    bool operator()(int a, int b) const {
        return a > b; // Sort in descending order
    }
};

map<int, string, ReverseSort> buyOrders;    // Buy orders sorted in descending order
map<int, string> sellOrders;                // Sell orders sorted in ascending order

// Order Matching Engine
// Time Priority
// Market order with no liquidity are rejected
void addOrder(string s) {
    Message m = Message(s);

    //TODO: Implementare XD
}

int main() {
    Message m = MessageBuilder(1, 1, "AAPL", Side::BUY, 100, OrderType::LIMIT).setPrice(100).build();

    m.display();

    cout << m.toFixMessage() << endl;

    Message m2 = Message(m.toFixMessage());

    m2.display();

    cout << m2.toFixMessage() << endl;

    return 0;
}