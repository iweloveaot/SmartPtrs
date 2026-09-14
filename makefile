# Makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = smart_ptr_app

# Исходные файлы
SRCS = main.cpp functional_tests.cpp load_tests.cpp
OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean run valgrind asan debug

all: $(TARGET)

# Компиляция с оптимизацией для корректных замеров времени
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Запуск обычного бинарника
run: $(TARGET)
	./$(TARGET)

# Запуск под Valgrind (проверка утечек памяти)
valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

# Компиляция и запуск с AddressSanitizer (ASan)
asan: 
	$(CXX) -std=c++17 -Wall -Wextra -g -fsanitize=address -fno-omit-frame-pointer $(SRCS) -o $(TARGET)_asan
	./$(TARGET)_asan

# Режим отладки (без оптимизаций, с дебаг-символами)
debug:
	$(CXX) -std=c++17 -Wall -Wextra -g -O0 $(SRCS) -o $(TARGET)_debug

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET)_asan $(TARGET)_debug benchmark_results.csv