#include<iostream>
#include<optional>
#include<sstream>
#include<vector>
#include"messages.hpp"
using namespace std;

int main() {
    Message m = MessageBuilder().setOrderId(1).setClientId(1).setInstrument("AAPL").setSide(Side::BUY).setQuantity(100).setType(OrderType::LIMIT).setPrice(100).build();

    m.display();

    cout << m.toFixMessage() << endl;

    Message m2 = Message(m.toFixMessage());

    m2.display();

    cout << m2.toFixMessage() << endl;

    return 0;
}