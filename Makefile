# Variables 

GXX = g++
GXXFLAGS = -g -Wall -Wextra

SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include
BIN_DIR = bin

SRC = $(wildcard $(SRC_DIR)/*.cpp) # Collect .cpp in SRC from SRC_DIR 
OBJ_SRC = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o, $(SRC)) # Convert all .cpp to .o

RUN_OBJ = $(OBJ_DIR)/run.o # Add run.o into OBJ manually
OBJ = $(OBJ_SRC) $(RUN_OBJ) # Build OBJ with run.o and other files .o

EXEC = $(BIN_DIR)/run # Add executable run into EXEC 

# make
all : $(EXEC)

# make start
start : $(EXEC)
	./$(EXEC)

# Linking
$(EXEC) : $(OBJ)
	$(GXX) $(OBJ) -o $@

# Compilation of .cpp to .o
$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(GXX) $(GXXFLAGS) -I$(INC_DIR) -c $< -o $@


# Compilation of run.cpp
$(OBJ_DIR)/run.o : run.cpp
	$(GXX) $(GXXFLAGS) -I$(INC_DIR) -c $< -o $@

# Cleaning
clean:
	rm -f $(OBJ) $(EXEC)

