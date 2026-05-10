#pragma once

#include <array>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

enum LogLevel {
    LOG_NORMAL = 0,
    LOG_WARNING = 1,
    LOG_ERROR = 2
};

class Log {
public:
    static Log* Instance() {
        static Log instance;
        return &instance;
    }

    void message(LogLevel level, const std::string& text) {
        entries_[next_] = Entry{Now(), level, text};
        next_ = (next_ + 1) % capacity_;
        if (size_ < capacity_) {
            ++size_;
        }
    }

    void print(std::ostream& os = std::cout) const {
        for (std::size_t i = 0; i < size_; ++i) {
            const std::size_t idx = (next_ + capacity_ - size_ + i) % capacity_;
            const Entry& e = entries_[idx];
            os << '[' << e.time << "] [" << LevelToString(e.level) << "] " << e.text << '\n';
        }
    }

private:
    struct Entry {
        std::string time;
        LogLevel level;
        std::string text;
    };

    static constexpr std::size_t capacity_ = 10;

    Log() = default;
    Log(const Log&) = delete;
    Log& operator=(const Log&) = delete;

    static const char* LevelToString(LogLevel level) {
        switch (level) {
            case LOG_NORMAL: return "NORMAL";
            case LOG_WARNING: return "WARNING";
            case LOG_ERROR: return "ERROR";
        }
        return "UNKNOWN";
    }

    static std::string Now() {
        const auto now = std::chrono::system_clock::now();
        const std::time_t tt = std::chrono::system_clock::to_time_t(now);
        std::tm tm{};
#ifdef _WIN32
        localtime_s(&tm, &tt);
#else
        localtime_r(&tt, &tm);
#endif
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    std::array<Entry, capacity_> entries_{};
    std::size_t next_ = 0;
    std::size_t size_ = 0;
};
