#pragma once

#include <fstream>
#include <string>

#include "error.h"

namespace al
{
        ////////////////////////////////////////////////////////////////////////////////
        inline std::string read(const std::string& url)
        {
                if (std::ifstream f{url}) {
                        std::string all;
                        for (std::string current; std::getline(f, current); all += current + '\n')
                                ;
                        return all;
                }
                throw exception("al", "", "read", "error reading file " + url, etype::unexpected);
        }
}

