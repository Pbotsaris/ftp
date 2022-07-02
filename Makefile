PROGRAM=ftp
BIN=bin
CFLAGS += -Wall -Wextra -g3 -Iinclude -fsanitize=address 
CPP_FLAGS = -Wpedantic -std=c++17 -Wcast-qual -Wnon-virtual-dtor -Woverloaded-virtual -Wold-style-cast
CC=clang++
RM=rm -rf

$(shell mkdir -p obj bin obj/server obj/client)

# SERVER ############
SERVER_TARGET=$(BIN)/$(PROGRAM)
SERVER_OBJ=obj
SERVER_SRC=src
SERVER_SRCS=$(wildcard $(SERVER_SRC)/*.cpp)
SERVER_OBJS=$(patsubst $(SERVER_SRC)/%.cpp, $(SERVER_OBJ)/%.o, $(SERVER_SRCS))

all: make_server

# SERVER ############
make_server: $(SERVER_TARGET)

$(SERVER_TARGET): $(SERVER_OBJS)
	$(CC) -o $(SERVER_TARGET) $(SERVER_OBJS) $(CFLAGS) $(CPP_FLAGS)

$(SERVER_OBJ)/%.o: $(SERVER_SRC)/%.cpp
	$(CC) $(CFLAGS) $(CPP_FLAGS) -c $< -o $@

run: ${SERVER_TARGET}
	./${SERVER_TARGET}

clean:
	$(RM) $(SERVER_TARGET) $(BIN)/*.dSYM $(SERVER_OBJ)/*.o 

.PHONY:
	all run clean 
