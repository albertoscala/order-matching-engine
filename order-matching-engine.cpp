#include<iostream>
#include<optional>
#include<sstream>
#include<vector>
#include<map>
#include<fstream>
#include"messages.hpp"
#include"transactions.hpp"
using namespace std;

// Custom comparator for reverse sorting
struct ReverseSort {
    bool operator()(int a, int b) const {
        return a > b; // Sort in descending order
    }
};

// Orderbooks
map<float, vector<string>, ReverseSort> buyOrders;    // Buy orders sorted in descending order
map<float, vector<string>> sellOrders;                // Sell orders sorted in ascending order
vector<Transaction> trades;                         // List of trades

void displayBuyOrdersVolumes() {
    int localVolume;

    cout << "BUY ORDERS" << endl;
    
    // Display the buy orders
    for (auto it = buyOrders.begin(); it != buyOrders.end(); it++) {
        cout << it->first << ": ";
        
        localVolume = 0;
        
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            localVolume += Message(*it2).getQuantity();
        }

        cout << localVolume << endl;
    }
}

void displaySellOrdersVolumes() {
    int localVolume;
    
    cout << "SELL ORDERS" << endl;

    // Display the sell orders
    for (auto it = sellOrders.begin(); it != sellOrders.end(); it++) {
        cout << it->first << ": ";
        
        localVolume = 0;
        
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            localVolume += Message(*it2).getQuantity();
        }

        cout << localVolume << endl;
    }

}

void downloadOrderbook() {
    // Download the orderbook transaction history
    for (auto it = trades.begin(); it != trades.end(); it++) {
        // Append the trade to the orderbook
        ofstream orderbook("orderbook.csv", ios::app);
        orderbook << it->inlineDisplay();
        orderbook.close();
    }
}

int checkBuyLiquidity() {
    // Counter for the total shares available
    int liquidity = 0;

    // Iterate over the buy orders
    for (auto it = buyOrders.begin(); it != buyOrders.end(); it++) {
        
        // Iterate over the orders at the current price level
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            
            // Add the quantity of the order to the total liquidity
            liquidity += Message(*it2).getQuantity();
        }
    }

    // Return the total liquidity
    return liquidity;
}

int checkSellLiquidity() {
    // Counter for the total shares available
    int liquidity = 0;

    // Iterate over the buy orders
    for (auto it = sellOrders.begin(); it != sellOrders.end(); it++) {
        
        // Iterate over the orders at the current price level
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            
            // Add the quantity of the order to the total liquidity
            liquidity += Message(*it2).getQuantity();
        }
    }

    // Return the total liquidity
    return liquidity;
}

int checkBuyLiquidityFromPrice(float price) {
    int liquidity = 0;

    // Display the buy orders
    for (auto it = buyOrders.begin(); it != buyOrders.end(); it++) {
        cout << it->first << ": ";
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            cout << Message(*it2).getQuantity() << " ";
        }
        cout << endl;
    }

    // Find the first price level greater or equal to the given price
    map<float, vector<string>>::iterator itPriceLimit = buyOrders.end();
    
    for (auto it = buyOrders.begin(); it != buyOrders.end(); it++) {
        if (it->first >= price) {
            // Get the iterator
            itPriceLimit = it;
        }
    }

    // Iterate over the buy orders
    for (auto it = itPriceLimit; it != buyOrders.end(); it++) {
        
        cout << it->first << endl;

        // Iterate over the orders at the current price level
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
            
            // Add the quantity of the order to the total liquidity
            liquidity += Message(*it2).getQuantity();
        }
    }

    return liquidity;
}

int checkSellLiquidityFromPrice(float price) {
    int liquidity = 0;

    // Find the first price level greater or equal to the given price
    map<float, vector<string>>::iterator itPriceLimit = sellOrders.end();

    for (auto it = sellOrders.begin(); it != sellOrders.end(); it++) {
        if (it->first >= price) {
            // Get the iterator
            itPriceLimit = it;
        }
    }

    // Iterate over the buy orders
    for (auto it = itPriceLimit; it != sellOrders.end(); it++) {

        // Iterate over the orders at the current price level
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {

            // Add the quantity of the order to the total liquidity
            liquidity += Message(*it2).getQuantity();
        }
    }

    return liquidity;
}

// Market Order Flows

// NO FUCKING CLUE I GUESS IT'S CORRECT IDK WHY BUT I THINK IT IS

// Market Buy Order Flow
void marketBuyFlow(Message m) {
    
    // Check if there is enough liquidity
    if (checkSellLiquidity() >= m.getQuantity()) { 
        
        // Go to the last price level
        for (auto it = sellOrders.end(); it != sellOrders.begin(); it--) {
            
            // Iterate over the sell orders
            for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
                
                // Get the order
                Message order = Message(*it2);

                // Check if the order quantity is greater than the market order quantity
                if (order.getQuantity() >= m.getQuantity()) {
                    
                    // Update the order quantity
                    order.setQuantity(order.getQuantity() - m.getQuantity());

                    // Check if the order quantity is zero
                    if (order.getQuantity() == 0) {
                        // Remove the order
                        sellOrders[it->first].erase(it2);
                    }

                    // Update the order inposition
                    sellOrders[it->first].insert(it2, order.toFixMessage());

                    // Record the trade
                    trades.push_back(Transaction(trades.size() + 1, m.getOrderId(), m.getClientId(), order.getClientId(), m.getInstrument(), m.getQuantity(), order.getPrice().value()));

                    // Break the loop
                    break;
                } else {
                    
                    // Update the market order quantity
                    m.setQuantity(m.getQuantity() - order.getQuantity());

                    // Remove the order
                    sellOrders[it->first].erase(it2);
                }
            }

            // Check if the market order quantity is zero
            if (m.getQuantity() == 0) {
                break;
            }
        }

    } else {
        cout << "Not enough liquidity" << endl;
    }
}

// Market Sell Order Flow
void marketSellFlow(Message m) {
    
    // Check if there is enough liquidity
    if (checkBuyLiquidity() >= m.getQuantity()) { 
        
        // Go to the last price level
        for (auto it = buyOrders.begin(); it != buyOrders.end(); it++) {
            
            // Iterate over the sell orders
            for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
                
                // Get the order
                Message order = Message(*it2);

                // Check if the order quantity is greater than the market order quantity
                if (order.getQuantity() >= m.getQuantity()) {
                    
                    // Update the order quantity
                    order.setQuantity(order.getQuantity() - m.getQuantity());

                    // Check if the order quantity is zero
                    if (order.getQuantity() == 0) {
                        // Remove the order
                        buyOrders[it->first].erase(it2);
                    }

                    // Update the order inposition
                    *it2 = order.toFixMessage();

                    // Record the trade
                    trades.push_back(Transaction(trades.size() + 1, m.getOrderId(), m.getClientId(), order.getClientId(), m.getInstrument(), m.getQuantity(), order.getPrice().value()));

                    // Break the loop
                    return;
                } else {
                    
                    // Update the market order quantity
                    m.setQuantity(m.getQuantity() - order.getQuantity());

                    // Remove the order
                    buyOrders[it->first].erase(it2);
                }
            }

            // Check if the market order quantity is zero
            if (m.getQuantity() == 0) {
                return;
            }
        }

    } else {
        cout << "Not enough liquidity" << endl;
    }

}

// Limit Order Flows

// Limit Buy Order Flow
void limitBuyFlow(Message m) {
    // Check if there is enough liquidity to fill the order
    if (checkSellLiquidityFromPrice(m.getPrice().value()) >= m.getQuantity()) {

        // Iterate over the sell orders
        for (auto it = sellOrders.lower_bound(m.getPrice().value()); it != sellOrders.end(); it++) {
            
            // Iterate over the orders at the current price level
            for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
                
                // Get the order
                Message order = Message(*it2);

                // Check if the order quantity is greater than the limit order quantity
                if (order.getQuantity() >= m.getQuantity()) {
                    
                    // Update the order quantity
                    order.setQuantity(order.getQuantity() - m.getQuantity());

                    // Check if the order quantity is zero
                    if (order.getQuantity() == 0) {
                        // Remove the order
                        sellOrders[it->first].erase(it2);
                    }

                    // Update the order inposition
                    *it2 = order.toFixMessage();

                    // Record the trade
                    trades.push_back(Transaction(trades.size() + 1, m.getOrderId(), m.getClientId(), order.getClientId(), m.getInstrument(), m.getQuantity(), order.getPrice().value()));

                    // Break the loop
                    break;
                } else {
                    
                    // Update the limit order quantity
                    m.setQuantity(m.getQuantity() - order.getQuantity());

                    // Remove the order
                    sellOrders[it->first].erase(it2);
                }
            }

            // Check if the limit order quantity is zero
            if (m.getQuantity() == 0) {
                break;
            }
        }

    } else {
        // Not enough liquidity to fill the order so add it to the order book
        buyOrders[m.getPrice().value()].push_back(m.toFixMessage());
    }

}

// Limit Sell Order Flow
void limitSellFlow(Message m) {
    // Check if there is enough liquidity to fill the order
    if (checkBuyLiquidityFromPrice(m.getPrice().value()) >= m.getQuantity()) {

        cout << checkBuyLiquidityFromPrice(m.getPrice().value()) << endl;

        cout << m.getQuantity() << endl;

        // Iterate over the buy orders
        for (auto it = buyOrders.lower_bound(m.getPrice().value()); it != buyOrders.end(); it++) {

            // Iterate over the orders at the current price level
            for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
                
                // Get the order
                Message order = Message(*it2);

                // Check if the order quantity is greater than the limit order quantity
                if (order.getQuantity() >= m.getQuantity()) {
                    
                    // Update the order quantity
                    order.setQuantity(order.getQuantity() - m.getQuantity());

                    // Check if the order quantity is zero
                    if (order.getQuantity() == 0) {
                        // Remove the order
                        buyOrders[it->first].erase(it2);
                    }

                    // Update the order inposition
                    buyOrders[it->first].insert(it2, order.toFixMessage());

                    // Record the trade
                    trades.push_back(Transaction(trades.size() + 1, m.getOrderId(), m.getClientId(), order.getClientId(), m.getInstrument(), m.getQuantity(), order.getPrice().value()));

                    // Break the loop
                    break;
                } else {
                    
                    // Update the limit order quantity
                    m.setQuantity(m.getQuantity() - order.getQuantity());

                    // Remove the order
                    buyOrders[it->first].erase(it2);
                }
            }

            // Check if the limit order quantity is zero
            if (m.getQuantity() == 0) {
                break;
            }
        }

    } else {
        // Not enough liquidity to fill the order so add it to the order book
        sellOrders[m.getPrice().value()].push_back(m.toFixMessage());
    }
}

// Order Matching Engine
// Time Priority
// Fill or Kill

void addOrder(string s) {
    Message m = Message(s);

    // Check the side of the order
    switch(m.getSide()) {
        case Side::BUY:

            switch(m.getType()) {
                case OrderType::MARKET:
                    marketBuyFlow(m);
                    break;
                case OrderType::LIMIT:
                    break;
            }

            break;
        case Side::SELL:

            switch(m.getType()) {
                case OrderType::MARKET:
                    marketSellFlow(m);
                    break;
                case OrderType::LIMIT:
                    break;
            }

            break;
    }

    // Capire se market o limit

    // Se market verificare se c'è liquidità

    // Se limit inserire nella coda del prezzo giusto
}

int main() {
    Message m = MessageBuilder(1, 1, "AAPL", Side::BUY, 100, OrderType::LIMIT).setPrice(100.5).build();

    Message m2 = MessageBuilder(2, 2, "AAPL", Side::BUY, 150, OrderType::LIMIT).setPrice(99.5).build();

    Message m3 = MessageBuilder(3, 3, "AAPL", Side::SELL, 200, OrderType::LIMIT).setPrice(101.5).build();

    limitBuyFlow(m);

    limitBuyFlow(m2);

    limitSellFlow(m3);

    displayBuyOrdersVolumes();

    displaySellOrdersVolumes();

    Message m4 = MessageBuilder(4, 4, "AAPL", Side::SELL, 50, OrderType::MARKET).build();

    marketSellFlow(m4);

    displayBuyOrdersVolumes();

    displaySellOrdersVolumes();

    Message m5 = MessageBuilder(5, 5, "AAPL", Side::SELL, 383, OrderType::LIMIT).setPrice(100).build();

    limitSellFlow(m5);

    displayBuyOrdersVolumes();

    displaySellOrdersVolumes();

    return 0;
}