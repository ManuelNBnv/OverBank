CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
SRCS = main.cpp src/ui.cpp src/users.cpp src/overbank.cpp src/storage.cpp src/funciones.cpp
TARGET = overbank

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) -lncurses

clean:
	rm -f $(TARGET) data/*.tmp
