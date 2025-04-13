# Compiler and flags
CXX = g++
CXXFLAGS = -g -Wall -Wextra -std=c++17 -O2
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Detect OS
OS := $(shell uname -s)

ifeq ($(OS), Linux)
    EXT = linux
    #CXXFLAGS += -static
else ifeq ($(OS), Darwin)
    EXT = mac
    CXXFLAGS += -stdlib=libc++
else ifeq ($(OS), Windows_NT)
    CXX := x86_64-w64-mingw32-g++
    EXT = exe
    CXXFLAGS += -static -D_WINDOWS
    LDFLAGS += -static-libgcc -static-libstdc++
endif

# Directories
SRC_DIR = Assignments
UTILS_DIR = Utils

# Default rule
all:
	@echo "Specify an assignment, e.g., make A1-linux, A1-windows, A2-mac"

# Generic build rule
define BUILD_TEMPLATE
$1: $$(OBJS_$1)
	@echo "Compiling and linking $$@..."
	$$(CXX) $$(CXXFLAGS) $$^ -o $1-$(EXT) $$(LDFLAGS)
endef

# Object files for A1 and A2
OBJS_A1 = $(SRC_DIR)/A1/main.o $(SRC_DIR)/A1/SA.o $(SRC_DIR)/A1/TS.o $(SRC_DIR)/A1/ProblemInstance.o
OBJS_A2 = $(SRC_DIR)/A2/main.o $(SRC_DIR)/A2/ProblemInstance.o

# Generate build rules
$(eval $(call BUILD_TEMPLATE,A1))
$(eval $(call BUILD_TEMPLATE,A2))

# Compilation rules for A1
$(SRC_DIR)/A1/main.o: $(SRC_DIR)/A1/main.cpp $(SRC_DIR)/A1/*.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SRC_DIR)/A1/SA.o: $(SRC_DIR)/A1/SA.cpp $(SRC_DIR)/A1/SA.h $(UTILS_DIR)/Logger.h
	@echo "Compiling SA..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SRC_DIR)/A1/TS.o: $(SRC_DIR)/A1/TS.cpp $(SRC_DIR)/A1/TS.h $(UTILS_DIR)/Logger.h
	@echo "Compiling TS..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SRC_DIR)/A1/ProblemInstance.o: $(SRC_DIR)/A1/ProblemInstance.cpp $(SRC_DIR)/A1/ProblemInstance.h $(UTILS_DIR)/Logger.h
	@echo "Compiling ProblemInstance..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compilation rules for A2
$(SRC_DIR)/A2/main.o: $(SRC_DIR)/A2/main.cpp $(SRC_DIR)/A2/*.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SRC_DIR)/A2/ProblemInstance.o: $(SRC_DIR)/A2/ProblemInstance.cpp $(SRC_DIR)/A2/ProblemInstance.h $(UTILS_DIR)/Logger.h
	@echo "Compiling ProblemInstance..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	@echo "Cleaning build files..."
	rm -f $(SRC_DIR)/*/*.o $(SRC_DIR)/*/*~ A1-linux A1-mac A1.exe A2-linux A2-mac A2.exe

# Run targets
runA1: A1-$(EXT)
	@echo "Running A1-$(EXT)..."
	./A1-$(EXT)

runA2: A2-$(EXT)
	@echo "Running A2-$(EXT)..."
	./A2-$(EXT)

# Platform-specific builds
A1-linux:
	$(MAKE) EXT=linux A1

A1-mac:
	$(MAKE) EXT=mac A1

A1-windows:
	$(MAKE) EXT=exe A1 CXX=x86_64-w64-mingw32-g++

A2-linux:
	$(MAKE) EXT=linux A2

A2-mac:
	$(MAKE) EXT=mac A2

A2-windows:
	$(MAKE) EXT=exe A2 CXX=x86_64-w64-mingw32-g++