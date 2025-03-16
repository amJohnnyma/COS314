/*

#include "Logger.h"

namespace fs = std::filesystem;

void logMessage(const std::string& level, const std::string& message, const std::string& filename) {
    // Get the current time to add a timestamp
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&now_time_t);

    // Format the timestamp
    std::ostringstream timestamp;
    timestamp << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

    // Ensure the logs directory exists
    fs::create_directories("Utils/logs");

    // Open the log file in append mode
    std::ofstream logFile("Utils/logs/" + filename, std::ios::app);

    if (logFile.is_open()) {
        // Write the timestamp, log level, and the message to the file
        logFile << "[" << timestamp.str() << "] [" << level << "] " << message << std::endl;
        logFile.close();
    } else {
        std::cerr << "Unable to open log file!" << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <LEVEL> <MESSAGE> <FILENAME>" << std::endl;
        return 1;
    }

    std::string level = argv[1];
    std::string message = argv[2];
    std::string filename = (argc > 3) ? argv[3] : "log.txt";

    logMessage(level, message, filename);

    return 0;
}


*/

