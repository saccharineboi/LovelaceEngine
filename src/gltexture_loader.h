#pragma once

#include "gltexture2D.h"

#include <string>
#include <unordered_map>

namespace al::gl
{
        ////////////////////////////////////////////////////////////////////////////////
        class texture_loader
        {
        private:
                std::unordered_map<std::string, texture2D> mTextures;

        public:
                ~texture_loader() { mTextures.clear(); }

                texture2D* load2D(const std::string& url);
        };
}
