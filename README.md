# Order Matching Engine

## Table of Contents
- [Order Matching Engine](#order-matching-engine)
  - [Table of Contents](#table-of-contents)
  - [Introduction](#introduction)
  - [Features](#features)
  - [Messages](#messages)

## Introduction

This is a simple order matching engine that matches buy and sell orders for a stock. The engine is implemented in C++.

## Features

The engine supports the following messages:

1. **Add Order**: Adds a new order to the order book.
   1. **Market Order**: A market order is an order to buy or sell a stock at the current market price. It does not have a price.
   2. **Limit Order**: A limit order is an order to buy or sell a stock at a specific price or better.
   3. **Stop Order**: A stop order is an order to buy or sell a stock once the price of the stock reaches a specified price, known as the stop price.
   4. **Limit Stop Order**: A limit stop order is an order to buy or sell a stock once the price of the stock reaches a specified price, known as the stop price, or better.

## Messages

The messages are in the following format:

```
orderId      // Unique identifier for the order
clientId     // Client placing the order
instrument   // Financial instrument
side         // Buy or sell
quantity     // NUmber of units to trade
type         // Order type
price        // Price for the limit or stop-limit orders
```

The messages are passed using the FIX message format.