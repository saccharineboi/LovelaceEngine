#include "glshader.h"
#include "error.h"

namespace al::gl
{
        ////////////////////////////////////////////////////////////////////////////////
        void shader::load()
        {
                mId = glCreateShader(mType);
                const char* source_str = mSource.c_str();
                glShaderSource(mId, 1, &source_str, nullptr);
                glCompileShader(mId);
        }

        ////////////////////////////////////////////////////////////////////////////////
        void shader::check()
        {
                int success;
                glGetShaderiv(mId, GL_COMPILE_STATUS, &success);
                if (!success) {
                        char infolog[512];
                        glGetShaderInfoLog(mId, 512, nullptr, infolog);
                        throw exception("al::gl", "shader", "check", infolog, etype::unexpected);
                }
        }

        ////////////////////////////////////////////////////////////////////////////////
        shader::shader(int type, const std::string& source)
                : mType{type}, mSource{source}
        {
                load();
                check();
        }

        ////////////////////////////////////////////////////////////////////////////////
        shader::shader(const shader& other)
                : mType{other.mType}, mSource{other.mSource}
        {
                load();
                check();
        }

        ////////////////////////////////////////////////////////////////////////////////
        shader& shader::operator=(const shader& other)
        {
                if (this != &other) {
                        glDeleteShader(mId);

                        mType   = other.mType;
                        mSource = other.mSource;

                        load();
                        check();
                }
                return *this;
        }

        ////////////////////////////////////////////////////////////////////////////////
        shader::shader(shader&& other)
                : mId{other.mId}, mType{other.mType}, mSource{std::move(other.mSource)}
        {
                other.mId = 0;
        }

        ////////////////////////////////////////////////////////////////////////////////
        shader& shader::operator=(shader&& other)
        {
                if (this != &other) {
                        glDeleteShader(mId);

                        mId     = other.mId;
                        mType   = other.mType;
                        mSource = std::move(other.mSource);

                        other.mId = 0;
                }
                return *this;
        }
}
