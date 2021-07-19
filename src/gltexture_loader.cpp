#include "gltexture_loader.h"
#include "log.h"

#include <unordered_map>
#include <string>

namespace al::gl::texture_loader
{
        ////////////////////////////////////////////////////////////////////////////////
        static std::unordered_map<std::string, texture2D> textures2D;
        static size_t totalBytesLoaded;

        ////////////////////////////////////////////////////////////////////////////////
        texture2D* load2D(const std::string& url)
        {
                auto tex = textures2D.find(url);
                if (tex == textures2D.end()) {
                        auto r = textures2D.emplace(url, std::move(texture2D(url)));
                        int sizeInBytes = r.first->second.getSizeInBytes();
                        totalBytesLoaded += sizeInBytes;
                        log(std::cout, __FILE__, __LINE__, "[Lovelace] [al::gl::texture_loader] Loaded ", url, " [", sizeInBytes, " bytes]");
                        return &r.first->second;
                }
                return &tex->second;
        }

        ////////////////////////////////////////////////////////////////////////////////
        size_t getTotalBytesLoaded()
        {
                return totalBytesLoaded;
        }

        ////////////////////////////////////////////////////////////////////////////////
        void clear()
        {
                textures2D.clear();
                log(std::cout, __FILE__, __LINE__, "[Lovelace] [al::gl::texture_loader] Cleared textures2D");
        }
}
