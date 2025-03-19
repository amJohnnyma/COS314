# COS314

Git repo: https://github.com/amJohnnyma/COS314

# README for TSP Algorithm Comparison

## Installation Instructions:
1. Clone the repository: `git clone https://github.com/amJohnnyma/COS314`
2. Navigate to the project directory: `cd <your-project-directory>`
3. Install dependencies: 
	3.1. Linux: 
sudo apt update
		Sudo apt install build-essential
	3.2. MacOS:
		xcode-select --install  # Install command line tools
brew install gcc    	# Install GCC (if not installed)
3.3. Windows:
	Install MinGW-w64 https://www.mingw-w64.org/
	Add minGW to your system PATH (or use Git Bash)

4. Run the program:
	./A1-linux
./A1-mac
./A1-windows
Or run the respective executable (for a random seed and 10 runs)

## Usage:
- If running from terminal:
Navigate to the project directory
Run ./A1-<respective OS>
Enter seed (If blank random seed, if a seed is entered then only 1 run will take place)
Enter the amount of runs
Open Utils/logs/runData.txt to find results
