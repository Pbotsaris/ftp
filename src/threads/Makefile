# UTILS
INCLUDE_FLAGS=-I../../include -I../../include/threads -I../../include/utils -I../../include/networking -I../../include/controllers
CFLAGS += -Wall -Wextra -g3 -fsanitize=address 
CPP_FLAGS = -Wpedantic -std=c++17 -Wcast-qual -Wnon-virtual-dtor -Woverloaded-virtual -Wold-style-cast
CC=clang++

SRC_DIR=.
OBJ_DIR=../../obj

SRCS=$(wildcard $(SRC_DIR)/*.cpp)
OBJS=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

all: $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	 $(CC) $(CFLAGS) $(CPP_FLAGS) $(INCLUDE_FLAGS) -c $< -o $@
