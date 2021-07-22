#pragma once

#include "config.h"

#include <iostream>
#include <chrono>
#include <cstring>

namespace al
{
        ////////////////////////////////////////////////////////////////////////////////
        template <typename... T>
        inline void log(std::ostream& os, const char* file, int line, T&&... values)
        {
#ifdef DEBUG
                os << '[' << file << ":" << line << "] ";
                (os << ... << values) << '\n';
#endif
        }
}

