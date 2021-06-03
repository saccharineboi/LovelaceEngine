#include "glprogram.h"
#include "error.h"

namespace al::gl
{
        ////////////////////////////////////////////////////////////////////////////////
        int program::findUniform(const std::string& name)
        {
                auto u = mUniforms.find(name);
                if (u == mUniforms.end()) {
                        int loc = glGetUniformLocation(mId, name.c_str());
                        if (loc == -1)
                                throw exception("al::gl", "program", "findUniform", "couldn't find " + name, etype::unexpected);
                        mUniforms.emplace(name, loc);
                        return loc;
                }
                else
                        return u->second;
        }

        ////////////////////////////////////////////////////////////////////////////////
        void program::check()
        {
                int success;
                glGetProgramiv(mId, GL_LINK_STATUS, &success);
                if (!success) {
                        char infolog[512];
                        glGetProgramInfoLog(mId, 512, nullptr, infolog);
                        throw exception("al::gl", "program", "check", infolog, etype::unexpected);
                }
        }

        ////////////////////////////////////////////////////////////////////////////////
        void program::load()
        {
                mId = glCreateProgram();
                for (const shader* s : mShaders)
                        glAttachShader(mId, s->getId());
                glLinkProgram(mId);
        }

        ////////////////////////////////////////////////////////////////////////////////
        program::program(std::initializer_list<shader*> shaders)
        {
                for (shader* s : shaders) {
#ifdef DEBUG
                        if (!s)
                                throw exception("al::gl", "program", "program", "received null shader in constructor", etype::unexpected);
#endif
                        mShaders.push_back(s);
                }

                load();
                check();
        }

        ////////////////////////////////////////////////////////////////////////////////
        program::program(const program& other)
                : mShaders{other.mShaders}, mUniforms{other.mUniforms}
        {
                load();
                check();
        }

        ////////////////////////////////////////////////////////////////////////////////
        program& program::operator=(const program& other)
        {
                if (this != &other) {
                        glDeleteProgram(mId);

                        mShaders        = other.mShaders;
                        mUniforms       = other.mUniforms;

                        load();
                        check();
                }
                return *this;
        }

        ////////////////////////////////////////////////////////////////////////////////
        program::program(program&& other)
                : mId{other.mId}, mShaders{std::move(other.mShaders)}, mUniforms{std::move(other.mUniforms)}
        {
                other.mId = 0;
        }

        ////////////////////////////////////////////////////////////////////////////////
        program& program::operator=(program&& other)
        {
                if (this != &other) {
                        glDeleteProgram(mId);

                        mId             = other.mId;
                        mShaders        = std::move(other.mShaders);
                        mUniforms       = std::move(other.mUniforms);

                        other.mId       = 0;
                }
                return *this;
        }
}
