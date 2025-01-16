CXX = clang++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2
SRC = order-matching-engine.cpp messages.cpp transactions.cpp
HEADERS = messages.hpp transactions.hpp
TARGET_A = order-matching-engine-arch
TARGET_M = order-matching-engine-macos

$(TARGET_A): $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET_A) $(SRC)

$(TARGET_M): $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET_M) $(SRC)

clean:
	rm -f $(TARGET)