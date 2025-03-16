#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>
#include <chrono>
#include <iomanip>

namespace fs = std::filesystem;

class Logger {
public:
    // Log info messages to a specific file
    template<typename T>
    static void info(const T& message, const std::string& filename = "log.txt") {
        log("INFO", message, filename);
    }

    // Log warning messages to a specific file
    template<typename T>
    static void warning(const T& message, const std::string& filename = "log.txt") {
        log("WARNING", message, filename);
    }

    // Log error messages to a specific file
    template<typename T>
    static void error(const T& message, const std::string& filename = "log.txt") {
        log("ERROR", message, filename);
    }

private:
    // Common log function that writes messages to a file
    template<typename T>
    static void log(const std::string& level, const T& message, const std::string& filename) {
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
};

#endif // LOGGER_H