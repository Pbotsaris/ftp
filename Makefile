PROGRAM=ftp
CFLAGS += -Wall -Wextra -g3 -Iinclude -fsanitize=address 
CPP_FLAGS = -Wpedantic -std=c++17 -Wcast-qual -Wnon-virtual-dtor -Woverloaded-virtual -Wold-style-cast
CC=clang++
RM=rm -rf
BIN_DIR=bin
OBJ_DIR=obj
SRC_DIR=src

TARGET=$(BIN_DIR)/$(PROGRAM)

all: compile_root compile_networking compile_controllers compile_utils link

link: 
	$(CC) -o $(TARGET) $(wildcard $(OBJ_DIR)/*.o) $(CFLAGS) $(CPP_FLAGS)

compile_root:
	 $(MAKE) -C $(SRC_DIR)

compile_networking:
	 $(MAKE) -C $(SRC_DIR)/networking

compile_controllers:
	 $(MAKE) -C $(SRC_DIR)/controllers

compile_utils:
	 $(MAKE) -C $(SRC_DIR)/utils


run: ${TARGET}
	./${TARGET}

clean:
	$(RM) $(TARGET) $(BIN_DIR)/*.dSYM $(OBJ_DIR)/*.o 

.PHONY:
	all run clean compile_root compile_utils compile_networking compile_controllers compile_root
