#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>

class Logger {
public:
    Logger(const std::string& filename, const std::string& header) {
        out.open(filename);
        if (!out.is_open()) {
            std::cerr << "Failed to open log file: " << filename << std::endl;
        } else {
            out << header << "\n";
            out << std::fixed << std::setprecision(6);
        }
    }

    ~Logger() {
        if (out.is_open()) out.close();
    }

    template<typename... Args>
    void log(Args... args) {
        if (out.is_open()) {
            int i = 0;
            ((out << (i++ == 0 ? "" : ",") << args), ...);
            out << "\n";
        }
    }

private:
    std::ofstream out;
};