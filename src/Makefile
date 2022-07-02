# root
CFLAGS += -Wall -Wextra -g3 -I../include -fsanitize=address 
CPP_FLAGS = -Wpedantic -std=c++17 -Wcast-qual -Wnon-virtual-dtor -Woverloaded-virtual -Wold-style-cast
CC=clang++

SRC_DIR=.
OBJ_DIR=../obj

SRCS=$(wildcard $(SRC_DIR)/*.cpp)
OBJS=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

all: $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	 $(CC) $(CFLAGS) $(CPP_FLAGS) -c $< -o $@

