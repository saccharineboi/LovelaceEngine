#include "glshader_loader.h"
#include "error.h"
#include "log.h"
#include "io.h"

#include <glad/glad.h>

#include <unordered_map>
#include <string>

namespace al::gl::shader_loader
{
        ////////////////////////////////////////////////////////////////////////////////
        static std::unordered_map<std::string, shader> shaders;

        ////////////////////////////////////////////////////////////////////////////////
        shader* load(int type, const std::string& url)
        {
                auto id = url + std::to_string(type);
                auto savedShader = shaders.find(id);
                if (savedShader == shaders.end()) {
                        auto shaderSource = std::string("#version 400 core\n");
                        shaderSource += [](int shaderType) {
                                switch (shaderType) {
                                        case GL_VERTEX_SHADER:
                                                return "#define VERTEX_SHADER\n";
                                        case GL_FRAGMENT_SHADER:
                                                return "#define FRAGMENT_SHADER\n";
                                        default:
                                                return "#define UNKNOWN_SHADER\n";
                                }
                        }(type);
                        shaderSource += read(url);
                        auto result = shaders.emplace(id, std::move(shader(type, shaderSource)));
                        log(std::cout, "[Lovelace] [al::gl::shader_loader] Loaded ", url);
                        return &result.first->second;
                }
                return &savedShader->second;
        }

        ////////////////////////////////////////////////////////////////////////////////
        void clear()
        {
                shaders.clear();
                log(std::cout, "[Lovelace] [al::gl::shader_loader] Cleared shaders");
        }
}
