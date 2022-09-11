COMPILER = clang++

FLAGS = -lncurses -std=c++17 -Iinclude 

SRC = src
SRCS = $(wildcard $(SRC)/*.cpp)

OBJ = obj
OBJS = $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRCS))

BINDIR = bin
BIN = $(BINDIR)/Debug/main

PROMPT = Make

all: $(BIN)

$(OBJ)/%.o: $(SRC)/%.cpp
	@echo [$(PROMPT)]: Compiling object files
	@$(COMPILER) -c $^ -o $@

$(BIN): $(OBJS)
	@echo [$(PROMPT)]: Building executable
	@$(COMPILER) $(FLAGS) $(OBJS) -g -o $@
	@echo [$(PROMPT)]: Done

run:
	@echo [$(PROMPT)]: Running
	@./bin/Debug/main

release:
	@echo [$(PROMPT)]: Removing old files
	@rm -rf $(BINDIR)/Release/* obj/*
	@echo [$(PROMPT)]: Compiling object files
	@$(COMPILER) $(FLAGS) -O3 -c $(SRCS)
	@mv *.o obj/.
	@echo [$(PROMPT)]: Building executable
	@$(COMPILER) $(OBJS) -O3 -o $(BINDIR)/Release/main
	@echo [$(PROMPT)]: Done

clean:
	rm -r $(OBJS) $(BINDIR)/Debug/* $(BINDIR)/Release/*
