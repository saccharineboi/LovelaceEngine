#pragma once

#include "glshader.h"

#include <string>
#include <unordered_map>

namespace al::gl
{
        ////////////////////////////////////////////////////////////////////////////////
        class shader_loader
        {
        private:
                std::unordered_map<std::string, shader> mShaders;

        public:
                ~shader_loader() { mShaders.clear(); }

                shader* load(int type, const std::string& url);
        };
}
