#include "glshader_loader.h"
#include "error.h"
#include "log.h"
#include "io.h"

#include <glad/glad.h>

namespace al::gl
{
        ////////////////////////////////////////////////////////////////////////////////
        shader* shader_loader::load(int type, const std::string& url)
        {
                auto id = url + std::to_string(type);
                auto savedShader = mShaders.find(id);
                if (savedShader == mShaders.end()) {
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
                        auto result = mShaders.emplace(id, std::move(shader(type, shaderSource)));
                        log(std::cout, __FILE__, __LINE__, "[Lovelace] [al::gl::shader_loader] Loaded ", url);
                        return &result.first->second;
                }
                return &savedShader->second;
        }

        ////////////////////////////////////////////////////////////////////////////////
        shader_loader::~shader_loader()
        {
                mShaders.clear();
                log(std::cout, __FILE__, __LINE__, "[Lovelace] [al::gl::shader_loader] Cleared shaders");
        }
}
