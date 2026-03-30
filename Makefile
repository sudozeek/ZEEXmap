# ZEEXmap Compilation Script
# Written by Zarak Afridi

CXX = g++
CXXFLAGS = -pthread -O3
TARGET = ZEEXmap
SRCS = main.cpp Scanner.cpp

all:
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)

