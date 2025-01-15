#include<iostream>
#include<optional>
#include<sstream>
#include<vector>
#include<map>
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

    // Check if the first price level greater or equal to the given price exists
    if (buyOrders.lower_bound(price) != buyOrders.end()) {
        
        // Iterate over the buy orders
        for (auto it = buyOrders.lower_bound(price); it != buyOrders.end(); it++) {
            
            // Iterate over the orders at the current price level
            for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
                
                // Add the quantity of the order to the total liquidity
                liquidity += Message(*it2).getQuantity();
            }
        }
    }

    return liquidity;
}

int checkSellLiquidityFromPrice(float price) {
    int liquidity = 0;

    // Check if the first price level greater or equal to the given price exists
    if (sellOrders.lower_bound(price) != sellOrders.end()) {
        
        // Iterate over the buy orders
        for (auto it = sellOrders.lower_bound(price); it != sellOrders.end(); it++) {
            
            // Iterate over the orders at the current price level
            for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
                
                // Add the quantity of the order to the total liquidity
                liquidity += Message(*it2).getQuantity();
            }
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
                    buyOrders[it->first].insert(it2, order.toFixMessage());

                    // Record the trade
                    trades.push_back(Transaction(trades.size() + 1, m.getOrderId(), m.getClientId(), order.getClientId(), m.getInstrument(), m.getQuantity(), order.getPrice().value()));

                    // Break the loop
                    break;
                } else {
                    
                    // Update the market order quantity
                    m.setQuantity(m.getQuantity() - order.getQuantity());

                    // Remove the order
                    buyOrders[it->first].erase(it2);
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
                    sellOrders[it->first].insert(it2, order.toFixMessage());

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

    m.display();

    cout << m.toFixMessage() << endl;

    Message m2 = Message(m.toFixMessage());

    m2.display();

    cout << m2.toFixMessage() << endl;

    return 0;
}