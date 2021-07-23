#include "gltexture_loader.h"
#include "log.h"

#include <string>

namespace al::gl
{
        ////////////////////////////////////////////////////////////////////////////////
        texture2D* texture_loader::load2D(const std::string& url)
        {
                auto tex = mTextures.find(url);
                if (tex == mTextures.end()) {
                        auto r = mTextures.emplace(url, std::move(texture2D(url)));
                        int sizeInBytes = r.first->second.getSizeInBytes();
                        log(std::cout, __FILE__, __LINE__, "[Lovelace] [al::gl::texture_loader] Loaded ", url, " [", sizeInBytes, " bytes]");
                        return &r.first->second;
                }
                return &tex->second;
        }
}
