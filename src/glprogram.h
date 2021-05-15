#pragma once

#include <glad/glad.h>
#include <initializer_list>
#include <vector>
#include <unordered_map>
#include <string>

#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glshader.h"

namespace al::gl
{
        ////////////////////////////////////////////////////////////////////////////////
        class program
        {
                unsigned mId;
                std::vector<shader*> mShaders;
                std::unordered_map<std::string, int> mUniforms;

                void load();
                void check();
                int findUniform(const std::string& name);
        public:
                program(std::initializer_list<shader*> shaders);

                ~program()                      { glDeleteProgram(mId); }

                program(const program&);
                program& operator=(const program&);

                program(program&&);
                program& operator=(program&&);

                unsigned getId() const          { return mId; }

                void use() const                { glUseProgram(mId); }
                void halt() const               { glUseProgram(0); }

                void uniform(const std::string& name, float x)                                  { glUniform1f(findUniform(name), x); }
                void uniform(const std::string& name, float x, float y)                         { glUniform2f(findUniform(name), x, y); }
                void uniform(const std::string& name, float x, float y, float z)                { glUniform3f(findUniform(name), x, y, z); }
                void uniform(const std::string& name, float x, float y, float z, float w)       { glUniform4f(findUniform(name), x, y, z, w); }

                void uniform(const std::string& name, int x)                                    { glUniform1i(findUniform(name), x); }
                void uniform(const std::string& name, int x, int y)                             { glUniform2i(findUniform(name), x, y); }
                void uniform(const std::string& name, int x, int y, int z)                      { glUniform3i(findUniform(name), x, y, z); }
                void uniform(const std::string& name, int x, int y, int z, int w)               { glUniform4i(findUniform(name), x, y, z, w); }

                void uniform(const std::string& name, const glm::vec2& v)                       { glUniform2fv(findUniform(name), 1, glm::value_ptr(v)); }
                void uniform(const std::string& name, const glm::vec3& v)                       { glUniform3fv(findUniform(name), 1, glm::value_ptr(v)); }
                void uniform(const std::string& name, const glm::vec4& v)                       { glUniform4fv(findUniform(name), 1, glm::value_ptr(v)); }

                void uniform(const std::string& name, const glm::mat2& m)                       { glUniformMatrix2fv(findUniform(name), 1, GL_FALSE, glm::value_ptr(m)); }
                void uniform(const std::string& name, const glm::mat3& m)                       { glUniformMatrix3fv(findUniform(name), 1, GL_FALSE, glm::value_ptr(m)); }
                void uniform(const std::string& name, const glm::mat4& m)                       { glUniformMatrix4fv(findUniform(name), 1, GL_FALSE, glm::value_ptr(m)); }
        };

        ////////////////////////////////////////////////////////////////////////////////
        inline bool operator==(const program& first, const program& second) { return first.getId() == second.getId(); }

        ////////////////////////////////////////////////////////////////////////////////
        inline bool operator!=(const program& first, const program& second) { return first.getId() != second.getId(); }
}

