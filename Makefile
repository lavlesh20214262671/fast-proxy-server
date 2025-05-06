CXX = g++
CXXFLAGS = -Wall -std=c++17
INCLUDES = -Iinclude
SRC_DIR = src
BIN_DIR = bin

all: create_dirs proxy_with_cache proxy_without_cache

create_dirs:
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)

proxy_with_cache: $(SRC_DIR)/proxy_server_with_cache.cpp $(SRC_DIR)/proxy_parse.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(BIN_DIR)/proxy_with_cache.exe $^ -lws2_32 -lstdc++ -lucrt -lpthread

proxy_without_cache: $(SRC_DIR)/proxy_server_without_cache.cpp $(SRC_DIR)/proxy_parse.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(BIN_DIR)/proxy_without_cache.exe $^ -lws2_32 -lstdc++ -lucrt -lpthread

clean:
	if exist $(BIN_DIR) rmdir /s /q $(BIN_DIR)
