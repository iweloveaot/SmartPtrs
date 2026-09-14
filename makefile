CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = smart_ptr_app

SRCS = main.cpp functional_tests.cpp load_tests.cpp
OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean run valgrind asan

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

asan: 
	$(CXX) -std=c++17 -Wall -Wextra -g -fsanitize=address -fno-omit-frame-pointer $(SRCS) -o $(TARGET)_asan
	./$(TARGET)_asan

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET)_asan benchmark_results.csv