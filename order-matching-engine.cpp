#include<iostream>
#include<optional>
using namespace std;

enum class Side {
    BUY,
    SELL
};

enum class OrderType {
    MARKET,
    LIMIT,
    STOP,
    STOP_LIMIT
};

class Message {
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

    string toFixMessage() {};

    void unpackFixMessage(string fixMessage) {};
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

    return 0;
}