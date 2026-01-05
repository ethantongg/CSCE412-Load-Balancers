# Makefile for Load Balancer Project
# Compiles all source files and creates executable

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = loadbalancer
OBJS = main.o LoadBalancer.o WebServer.o

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile main.cpp
main.o: main.cpp LoadBalancer.h WebServer.h Request.h
	$(CXX) $(CXXFLAGS) -c main.cpp

# Compile LoadBalancer.cpp
LoadBalancer.o: LoadBalancer.cpp LoadBalancer.h WebServer.h Request.h
	$(CXX) $(CXXFLAGS) -c LoadBalancer.cpp

# Compile WebServer.cpp
WebServer.o: WebServer.cpp WebServer.h Request.h
	$(CXX) $(CXXFLAGS) -c WebServer.cpp

# Clean up compiled files
clean:
	rm -f $(OBJS) $(TARGET) loadbalancer_log.txt

# Run the program with default settings (10 servers, 10000 cycles)
run: $(TARGET)
	echo "10\n10000" | ./$(TARGET)

# Generate documentation with Doxygen
docs:
	doxygen Doxyfile

# Clean documentation
clean-docs:
	rm -rf html/ latex/

.PHONY: all clean run docs clean-docs