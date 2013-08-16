BIN=daemon
OBJ=Program.o utils.o Daemon.o
CXX=g++ -Wall

TEST_BIN=test

all:$(BIN)

$(BIN):$(OBJ)
	$(CXX) -o $@ $^ main.cpp
%.o%.cpp:
	$(CXX) -c $^ -o $@

$(TEST_BIN):
	$(CXX) -o $@ $^ test.cpp
clean:
	rm -rf $(OBJ) $(BIN) $(TEST_BIN)

.PHONY: daemon test1


