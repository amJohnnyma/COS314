# Compiler and flags
CXX = g++
CXXFLAGS = -g -Wall -Wextra -std=c++17 -O2
LDFLAGS =
TARGET_BASE = A1

# Detect OS
OS := $(shell uname -s)

ifeq ($(OS), Linux)
    TARGET := $(TARGET_BASE)-linux
    CXXFLAGS += -static
else ifeq ($(OS), Darwin)  # macOS
    TARGET := $(TARGET_BASE)-mac
    CXXFLAGS += -stdlib=libc++
else ifeq ($(OS), Windows_NT)  # Windows
    CXX := x86_64-w64-mingw32-g++
    TARGET := $(TARGET_BASE).exe
    CXXFLAGS += -static -D_WINDOWS
    LDFLAGS += -static-libgcc -static-libstdc++
endif

# Directories
SRC_DIR = Assignments
UTILS_DIR = Utils

# Default rule
all:
	@echo "Specify an assignment, e.g., make A1-linux, A1-windows, A1-mac"

# Build rule for any assignment
A1: $(SRC_DIR)/A1/main.o $(SRC_DIR)/A1/SA.o $(SRC_DIR)/A1/TS.o $(SRC_DIR)/A1/ProblemInstance.o
	@echo "Compiling and linking $@..."
	$(CXX) $(CXXFLAGS) $^ -o $(TARGET) $(LDFLAGS)

# Compile main files
$(SRC_DIR)/A1/main.o: $(SRC_DIR)/A1/main.cpp $(SRC_DIR)/A1/*.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile SA and TS, ensuring Logger.h is included
$(SRC_DIR)/A1/SA.o: $(SRC_DIR)/A1/SA.cpp $(SRC_DIR)/A1/SA.h $(UTILS_DIR)/Logger.h
	@echo "Compiling SA..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SRC_DIR)/A1/TS.o: $(SRC_DIR)/A1/TS.cpp $(SRC_DIR)/A1/TS.h $(UTILS_DIR)/Logger.h
	@echo "Compiling TS..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SRC_DIR)/A1/ProblemInstance.o: $(SRC_DIR)/A1/ProblemInstance.cpp $(SRC_DIR)/A1/ProblemInstance.h $(UTILS_DIR)/Logger.h
	@echo "Compiling ProblemInstance..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	@echo "Cleaning build files..."
	rm -f $(SRC_DIR)/*/*.o $(SRC_DIR)/*/*~ $(TARGET) $(TARGET).exe

# Run the program
run: A1
	@echo "Running $(TARGET)..."
	./$(TARGET) $(ARGS)

# Platform-specific builds
A1-linux:
	$(MAKE) A1 TARGET=$(TARGET_BASE)-linux CXXFLAGS="$(CXXFLAGS) -static"

A1-mac:
	$(MAKE) A1 TARGET=$(TARGET_BASE)-mac CXXFLAGS="$(CXXFLAGS) -stdlib=libc++"

A1-windows:
	$(MAKE) A1 TARGET=$(TARGET_BASE).exe CXX=x86_64-w64-mingw32-g++ CXXFLAGS="$(CXXFLAGS) -D_WINDOWS"
