#include <unordered_map>
#include <glad/glad.h>
#include <string>

#include "glshader_loader.h"
#include "error.h"
#include "log.h"
#include "io.h"

namespace al::gl::shader_loader
{
        ////////////////////////////////////////////////////////////////////////////////
        static std::unordered_map<std::string, shader> shaders;

        ////////////////////////////////////////////////////////////////////////////////
        shader* load(const std::string& url)
        {
                auto s = shaders.find(url);
                if (s == shaders.end()) {
                        int type = [](auto& path) {
                                if (path.find(".vert") != std::string::npos)
                                        return GL_VERTEX_SHADER;
                                else if (path.find(".frag") != std::string::npos)
                                        return GL_FRAGMENT_SHADER;
                                else
                                        throw exception("al::gl::shader_loader", "none", "load", "shader type unknown", etype::unexpected);
                        }(url);
                        auto r = shaders.emplace(url, std::move(shader(type, read(url))));
                        log(std::cout, "[Lovelace] [al::gl::shader_loader] Loaded ", url); 
                        return &r.first->second;
                }
                return &s->second;
        }

        ////////////////////////////////////////////////////////////////////////////////
        void clear()
        {
                shaders.clear();
                log(std::cout, "[Lovelace] [al::gl::shader_loader] Cleared shaders");
        }
}
