# Compiler and flags
CXX = g++
CXXFLAGS = -g -Wall -Wextra -std=c++17 -O2

# Directories
SRC_DIR = Assignments
UTILS_DIR = Utils/logs

# Default rule (empty to force specifying a target)
all:
	@echo "Specify an assignment, e.g., make A1"

# Build rule for any assignment (e.g., A1)
%: $(SRC_DIR)/%/main.o $(SRC_DIR)/%/SA.o $(SRC_DIR)/%/TS.o $(SRC_DIR)/%/ProblemInstance.o
	@echo "Compiling and linking $@..."
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile main files
$(SRC_DIR)/%/main.o: $(SRC_DIR)/%/main.cpp $(SRC_DIR)/%/*.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile SA and TS, ensuring Logger.h is included
$(SRC_DIR)/%/SA.o: $(SRC_DIR)/%/SA.cpp $(SRC_DIR)/%/SA.h $(UTILS_DIR)/Logger.h
	@echo "Compiling SA..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SRC_DIR)/%/TS.o: $(SRC_DIR)/%/TS.cpp $(SRC_DIR)/%/TS.h $(UTILS_DIR)/Logger.h
	@echo "Compiling TS..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SRC_DIR)/%/ProblemInstance.o: $(SRC_DIR)/%/ProblemInstance.cpp $(SRC_DIR)/%/ProblemInstance.h $(UTILS_DIR)/Logger.h
	@echo "Compiling ProblemInstance..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	@echo "Cleaning build files..."
	rm -f $(SRC_DIR)/*/*.o $(SRC_DIR)/*/*~ $(SRC_DIR)/*/A*

# Build and run on command `make run-A1`
run-%: %
	@echo "Running $@..."
	./$<
