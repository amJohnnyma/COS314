# COS314 - TSP Algorithm Comparison

## Git Repository
[GitHub Repository](https://github.com/amJohnnyma/COS314)

---

## Reports
- [A1](https://github.com/amJohnnyma/COS314/blob/main/COS314A1.pdf) (Please download for best experience)
- [A2]() (Please download for best experience)

---

## Installation Instructions

#### 🔹 **Github Codespace (Recommended)**
```sh
make A1-linux (or A2, A3 etc.)
make runA1
```

### 1 Clone the Repository
```sh
git clone https://github.com/amJohnnyma/COS314
cd COS314
```

### 2 Install Dependencies

#### 🔹 **Linux (Ubuntu/Debian-based, or WSL)**
```sh
sudo apt update
sudo apt install build-essential
make A1-linux
```

#### 🔹 **macOS**
```sh
xcode-select --install  # Install command line tools
brew install gcc        # Install GCC (if not installed)
make A1-mac
```

#### 🔹 **Windows**
1. Download and install **MinGW-w64**: [MinGW-w64 Website](https://www.mingw-w64.org/)
2. Add MinGW to your system `PATH` (or use **Git Bash**)
make A1-windows

---

## Running the Program

### **On Linux**
```sh
make runA1
```

### **On macOS**
```sh
./A1-mac
```

### **On Windows**
```sh
A1-windows.exe
```

Alternatively, run the respective executable file directly.

**By default**, the program runs with a **random seed** and **10 iterations**.

---

## Usage Instructions

### **A1**
### **Running from Terminal**
1. **Navigate to the project directory**
2. Run the appropriate executable for your OS:
   ```sh
   ./A1-linux  # For Linux
   ./A1-mac    # For macOS
   A1-windows  # For Windows
   ```
3. **Enter Seed Value:**
   - Leave **blank** for a **random seed**.
   - Enter a **specific seed** to run with a fixed value (**1 run only**).
4. **Enter the number of runs** (if a random seed was chosen).
5. **Check the results** in:
   ```sh
   Utils/logs/runData.txt
   ```

### **A2**
### **Running from Terminal**
1. **Navigate to the project directory**
2. Run the appropriate executable for your OS:
   ```sh
   ./A2-linux  # For Linux
   ./A2-mac    # For macOS
   A2-windows  # For Windows
   ```

3. **Check the results** in:
   ```sh
   Utils/logs/results.txt
   ```
---

## Output Location
- The results are stored in: `Utils/logs/runData.txt`
- Open the file to analyze the algorithm's performance.

---

### **Now You're Ready to Run the assignments algorithms Algorithms! **

For any issues, feel free to check the repository or raise an issue. 

