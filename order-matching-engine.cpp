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
    Message(optional<int> orderId, optional<int> clientId, optional<string> instrument, optional<int> side, optional<int> quantity, optional<int> type, optional<int> price, optional<int> stopPrice) {
        if (orderId.has_value()) {
            this->orderId = orderId;
        }
        if (clientId.has_value()) {
            this->clientId = clientId;
        }
        if (instrument.has_value()) {
            this->instrument = instrument;
        }
        if (side.has_value()) {
            this->side = side;
        }
        if (quantity.has_value()) {
            this->quantity = quantity;
        }
        if (type.has_value()) {
            this->type = type;
        }
        if (price.has_value()) {
            this->price = price;
        }
        if (stopPrice.has_value()) {
            this->stopPrice = stopPrice;
        }
    };

    Message(string fixMessage) {
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
                this->side = stoi(keyValue[1]);
            } else if (keyValue[0] == "38") {
                this->quantity = stoi(keyValue[1]);
            } else if (keyValue[0] == "40") {
                this->type = stoi(keyValue[1]);
            } else if (keyValue[0] == "44") {
                this->price = stoi(keyValue[1]);
            } else if (keyValue[0] == "99") {
                this->stopPrice = stoi(keyValue[1]);
            }
        }
    };

    void display() {
        if (orderId.has_value()) {
            cout << "Order Id: " << orderId.value() << endl;
        }

        if (clientId.has_value()) {
            cout << "Client Id: " << clientId.value() << endl;
        }

        if (instrument.has_value()) {
            cout << "Instrument: " << instrument.value() << endl;
        }

        if (side.has_value()) {
            cout << "Side: " << side.value() << endl;
        }

        if (quantity.has_value()) {
            cout << "Quantity: " << quantity.value() << endl;
        }

        if (type.has_value()) {
            cout << "Type: " << type.value() << endl;
        }

        if (price.has_value()) {
            cout << "Price: " << price.value() << endl;
        }

        if (stopPrice.has_value()) {
            cout << "Stop Price: " << stopPrice.value() << endl;
        }

    };

    string toFixMessage() {
        string fixMessage = "";

        if (orderId.has_value()) {
            fixMessage += "11=" + to_string(orderId.value()) + "|";
        }

        if (clientId.has_value()) {
            fixMessage += "1=" + to_string(clientId.value()) + "|";
        }

        if (instrument.has_value()) {
            fixMessage += "55=" + instrument.value() + "|";
        }

        if (side.has_value()) {
            fixMessage += "54=" + to_string(side.value()) + "|";
        }

        if (quantity.has_value()) {
            fixMessage += "38=" + to_string(quantity.value()) + "|";
        }

        if (type.has_value()) {
            fixMessage += "40=" + to_string(type.value()) + "|";
        }

        if (price.has_value()) {
            fixMessage += "44=" + to_string(price.value()) + "|";
        }

        if (stopPrice.has_value()) {
            fixMessage += "99=" + to_string(stopPrice.value()) + "|";
        }

        return fixMessage;
    };

    void unpackFixMessage(string fixMessage) {
        

    };
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
    MessageBuilder& setOrderId(int orderId) {
        this->orderId = orderId;
        return *this;
    }

    MessageBuilder& setClientId(int clientId) {
        this->clientId = clientId;
        return *this;
    }

    MessageBuilder& setInstrument(string instrument) {
        this->instrument = instrument;
        return *this;
    }

    MessageBuilder& setSide(Side side) {
        this->side = static_cast<int>(side);
        return *this;
    }

    MessageBuilder& setQuantity(int quantity) {
        this->quantity = quantity;
        return *this;
    }

    MessageBuilder& setType(OrderType type) {
        this->type = static_cast<int>(type);
        return *this;
    }

    MessageBuilder& setPrice(int price) {
        this->price = price;
        return *this;
    }

    MessageBuilder& setStopPrice(int stopPrice) {
        this->stopPrice = stopPrice;
        return *this;
    }

    Message build() {
        return Message(orderId, clientId, instrument, side, quantity, type, price, stopPrice);
    }
};

int main() {
    Message m = MessageBuilder().setOrderId(1).setClientId(1).setInstrument("AAPL").setSide(Side::BUY).setQuantity(100).setType(OrderType::LIMIT).setPrice(100).build();

    m.display();

    cout << m.toFixMessage() << endl;

    Message m2 = Message(m.toFixMessage());

    m2.display();

    cout << m2.toFixMessage() << endl;

    return 0;
}

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