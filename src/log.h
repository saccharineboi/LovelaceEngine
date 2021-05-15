#pragma once

#include <iostream>
#include <chrono>
#include <cstring>

namespace al
{
        ////////////////////////////////////////////////////////////////////////////////
        template <typename... T>
        inline void log(std::ostream& os, T&&... values)
        {
                auto now = std::chrono::system_clock::now();
                std::time_t current = std::chrono::system_clock::to_time_t(now);
#define LOG_BUFFER_SIZE 32
                char timeStr[LOG_BUFFER_SIZE];
                strcpy(timeStr, ctime(&current));
                timeStr[strlen(timeStr) - 1] = ']';
                os << "[" << timeStr << '\t';
                (os << ... << values) << '\n';
        }
}

