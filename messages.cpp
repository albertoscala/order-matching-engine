#include<iostream>
#include<optional>
#include<sstream>
#include<vector>
#include"messages.hpp"
using namespace std;

// Helper function to split a string by a delimiter
vector<string> split(string s, char delimiter) {
    vector<string> fields;
    stringstream ss(s);
    string field;

    // Split by delimiter
    while (getline(ss, field, delimiter)) {
        fields.push_back(field);
    }

    return fields;
}

// Message class methods
Message::Message(int orderId, int clientId, string instrument, Side side, int quantity, OrderType type, optional<int> price) {

    this->orderId = orderId;

    this->clientId = clientId;

    this->instrument = instrument;

    this->side = side;

    this->quantity = quantity;

    this->type = type;

    if (price.has_value()) {
        this->price = price;
    }
};

Message::Message(string fixMessage) {
    vector<string> fields = split(fixMessage, '|');

    for (string field : fields) {
        vector<string> keyValue = split(field, '=');

        // Awesome error handling
        if (keyValue.size() != 2) {
            continue;
        }

        if (keyValue[0] == "11") {
            this->orderId = stoi(keyValue[1]);
        } else if (keyValue[0] == "1") {
            this->clientId = stoi(keyValue[1]);
        } else if (keyValue[0] == "55") {
            this->instrument = keyValue[1];
        } else if (keyValue[0] == "54") {
            this->side = static_cast<Side>(stoi(keyValue[1]));
        } else if (keyValue[0] == "38") {
            this->quantity = stoi(keyValue[1]);
        } else if (keyValue[0] == "40") {
            this->type = static_cast<OrderType>(stoi(keyValue[1]));
        } else if (keyValue[0] == "44") {
            this->price = stoi(keyValue[1]);
        }
    }
};

void Message::display() {

    cout << "Order Id: " << orderId << endl;

    cout << "Client Id: " << clientId << endl;

    cout << "Instrument: " << instrument << endl;

    cout << "Side: " << static_cast<int>(side) << endl;

    cout << "Quantity: " << quantity << endl;

    cout << "Type: " << static_cast<int>(type) << endl;

    if (price.has_value()) {
        cout << "Price: " << price.value() << endl;
    }

};

string Message::toFixMessage() {
    string fixMessage = "";

    fixMessage += "11=" + to_string(orderId) + "|";

    fixMessage += "1=" + to_string(clientId) + "|";

    fixMessage += "55=" + instrument + "|";

    fixMessage += "54=" + to_string(static_cast<int>(side)) + "|";

    fixMessage += "38=" + to_string(quantity) + "|";

    fixMessage += "40=" + to_string(static_cast<int>(type)) + "|";

    if (price.has_value()) {
        fixMessage += "44=" + to_string(price.value()) + "|";
    }

    return fixMessage;
};

// MessageBuilder class methods
MessageBuilder::MessageBuilder(int orderId, int clientId, string instrument, Side side, int quantity, OrderType type) {
    this->orderId = orderId;
    this->clientId = clientId;
    this->instrument = instrument;
    this->side = side;
    this->quantity = quantity;
    this->type = type;
};

MessageBuilder& MessageBuilder::setPrice(int price) {
    this->price = price;
    return *this;
}

Message MessageBuilder::build() {
    return Message(orderId, clientId, instrument, side, quantity, type, price);
}