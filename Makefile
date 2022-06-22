CLIENT=client
SERVER=server


PROGRAM=ftp
BIN=bin
CFLAGS += -Wall -Wextra -g3 -Iinclude -fsanitize=address 
CPP_FLAGS = -Wpedantic -std=c++17 -Wcast-qual -Wnon-virtual-dtor -Woverloaded-virtual -Wold-style-cast
CC=clang++
RM=rm -rf


$(shell mkdir -p obj bin obj/server obj/client)

# SERVER ############
SERVER_TARGET=$(BIN)/$(PROGRAM)
SERVER_OBJ=obj/server
SERVER_SRC=src/server
SERVER_SRCS=$(wildcard $(SERVER_SRC)/*.cpp)
SERVER_OBJS=$(patsubst $(SERVER_SRC)/%.cpp, $(SERVER_OBJ)/%.o, $(SERVER_SRCS))

# CLIENT ############
CLIENT_TARGET=$(BIN)/$(PROGRAM)_client
CLIENT_OBJ=obj/client
CLIENT_SRC=src/client
CLIENT_SRCS=$(wildcard $(CLIENT_SRC)/*.cpp)
CLIENT_OBJS=$(patsubst $(CLIENT_SRC)/%.cpp, $(CLIENT_OBJ)/%.o, $(CLIENT_SRCS))

all: make_server make_client

# SERVER ############
make_server: $(SERVER_TARGET)

$(SERVER_TARGET): $(SERVER_OBJS)
	$(CC) -o $(SERVER_TARGET) $(SERVER_OBJS) $(CFLAGS) $(CPP_FLAGS)

$(SERVER_OBJ)/%.o: $(SERVER_SRC)/%.cpp
	$(CC) $(CFLAGS) $(CPP_FLAGS) -c $< -o $@

# CLIENT ############
make_client: $(CLIENT_TARGET)

$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CC) -o $(CLIENT_TARGET) $(CLIENT_OBJS) $(CFLAGS) $(CPP_FLAGS)

$(CLIENT_OBJ)/%.o: $(CLIENT_SRC)/%.cpp
	$(CC) $(CFLAGS) $(CPP_FLAGS) -c $< -o $@

run: ${SERVER_TARGET}
	./${SERVER_TARGET}

clean:
	$(RM) $(SERVER_TARGET) $(BIN)/*.dSYM $(SERVER_OBJ)/*.o  $(CLIENT_OBJ)/*.o 

.PHONY:
	all run clean 
