# Order Matching Engine

## Table of Contents
- [Order Matching Engine](#order-matching-engine)
  - [Table of Contents](#table-of-contents)
  - [Introduction](#introduction)
  - [Features](#features)
  - [Messages](#messages)
    - [Functions](#functions)
      - [toFixMessage()](#tofixmessage)
      - [Message(string fixMessage)](#messagestring-fixmessage)
  - [Transactions](#transactions)
  - [Order Book](#order-book)
  - [Order Flows and Matching](#order-flows-and-matching)
    - [Functions](#functions-1)
      - [checkLiquidity()](#checkliquidity)
        - [checkBuyLiquidity()](#checkbuyliquidity)
        - [checkSellLiquidity()](#checksellliquidity)
      - [checkLiquidityFromPrice(float price)](#checkliquidityfrompricefloat-price)
        - [checkBuyLiquidityFromPrice(float price)](#checkbuyliquidityfrompricefloat-price)
        - [checkSellLiquidityFromPrice(float price)](#checksellliquidityfrompricefloat-price)

## Introduction

This is a simple order matching engine that I decided to build in order to have a better understanding on how a stock exchange works. The engine is built using the C++ programming language and is based on the FIX protocol.
It has many limitations and is not meant to fully emulate a real stock exchange. It is just a simple project to help me understand the basics of a stock exchange.

## Features

The engine currently supports only two type of orders: 

1. **Market Order**: A market order is an order to buy or sell a stock at the current market price. It does not have a price.  
2. **Limit Order**: A limit order is an order to buy or sell a stock at a specific price or better.

The transaction policy currently implemented is the simplest one: **First In First Out (FIFO)** priority with a **Fill or Kill (FOK)** policy. This means that the first order to arrive at the engine will be the first to be executed and if the order cannot be fully executed, it will be cancelled if a market order or will be placed in the order book if a limit order.

## Messages

The messages are the way the clients interact with the engine. Each order is a message that is sent to the engine.

The messages are in the following format:

```cpp
int orderId             // Unique identifier for the order
int clientId            // Client placing the order
string instrument       // Financial instrument
Side/int side           // Buy or sell
int quantity            // Number of units to trade
Type/int type           // Order type
optional<float> price   // Price for the limit or stop-limit orders
```

To avoid confusion during the writing of the code I created an enum for the side and type of the order.

```cpp
enum class Side {
    BUY = 1,
    SELL = 2
};

enum class OrderType {
    MARKET = 1,
    LIMIT = 2
};
```

### Functions

Other than the classic functions needed such as the constructor and the various setters and getters we have the following functions:

#### toFixMessage()

```cpp
string toFixMessage();
```

This function is used to convert the message to a string in the [FIX format](https://en.wikipedia.org/wiki/Financial_Information_eXchange). The enum values have a corresponding integer value that can be used to convert the enum to an integer and vice-versa and all the other message values are already easily convertible to a string.

#### Message(string fixMessage)

```cpp
Message(string fixMessage);
```

This is another option to the default constructor that allows to create a message from a string in the [FIX format](https://en.wikipedia.org/wiki/Financial_Information_eXchange). This is useful when we have to read the messages from a file or from a socket (coming soon).

## Transactions

Once we setup the messages and we have a match between a buy and a sell order, we generate a transaction.

The transactions are in the following format:

```cpp
int             tradeId;            // Unique identifier for the order
int             orderId;            // Client placing the order
int             buyerId;            // Client placing the order
int             sellerId;           // Client placing the order
string          instrument;         // Financial instrument
int             quantity;           // Number of units to trade
float           price;              // Price for the limit or stop-limit orders
long long       timestamp;          // Time of the transaction
```

It's possible to actually download the transactions in a CSV file.

## Order Book

The order book is a simple data structure that holds the orders that are not yet executed. It is implemented using hash maps and linked lists.
We have the key as the price of the order and the value as a linked list of orders.

There are two order books: one for the buy orders and one for the sell orders.

```cpp
map<float, vector<string>, ReverseSort> buyOrders;    // Buy orders sorted in descending order
map<float, vector<string>> sellOrders;                // Sell orders sorted in ascending order
```

The buy orders are sorted in descending order and the sell orders are sorted in ascending order. This is the same as it is implemented in a real stock exchange and it does provides actual benefits during the code writing.

## Order Flows and Matching

There are 4 order flows to be considered:

- **Market Buy Order**: A market buy order is an order to buy a stock at the current market price.
- **Market Sell Order**: A market sell order is an order to sell a stock at the current market price.
- **Limit Buy Order**: A limit buy order is an order to buy a stock at a specific price or better.
- **Limit Sell Order**: A limit sell order is an order to sell a stock at a specific price or better.

Before going through the matching process, we have to understand what the **Fill or Kill (FOK)** policy is.

For every **Market Order** we have to check if there are enough orders to be executed, if there aren't we have to cancel the order.

For every **Limit Order** we have to check if there are enough orders to be executed, if there aren't we have to place the order in the right order book.

The matching process is the same for all the order flows with some minor differences:

- Check if there is enough liquidity for the order to be executed.
  - For the **Market Order** the liquidity avaible is the same as the total liquidity for the order book.
  - While for the **Limit Order** the liquidity to consider is only the open with a price higher or equal to the order price.
- Then we have to execute the matching process.
  - For both the orders: We start from the best price available and we continue filling the orders until we reach the desired quantity.

### Functions

There are some very useful functions that I designed to help with the matching process:

#### checkLiquidity()

This function is used to check the liquidity available for the order. Mainly designed for the market orders. It could be implemented by default in the next one but for the moment they are separated functions.

There are of course two functions, one for the buy orders and one for the sell orders. Same as above, they could be all one single function but it is what it is.

#### checkBuyLiquidity()

```cpp
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
```

#### checkSellLiquidity()

```cpp
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
```

### checkLiquidityFromPrice(float price)

These functions are mainly designed for the limit orders. They are used to check the liquidity available for the order at a specific price. They will consider only the liquidity available at the price level or better.

#### checkBuyLiquidityFromPrice(float price)

```cpp
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
```

#### checkSellLiquidityFromPrice(float price)

```cpp
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
```