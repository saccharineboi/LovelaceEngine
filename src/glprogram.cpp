#include "glprogram.h"
#include "error.h"

#include <vector>

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
                for (const shader* s : mShaders) {
                        if (!s)
                                throw exception("al::gl", "program", "load", "one of the provided shaders is nullptr", etype::unexpected);
                        glAttachShader(mId, s->getId());
                }
                glLinkProgram(mId);
        }

        ////////////////////////////////////////////////////////////////////////////////
        program::program(std::initializer_list<shader*> shaders)
        {
                std::copy(shaders.begin(), shaders.end(), std::back_inserter(mShaders));
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

        ////////////////////////////////////////////////////////////////////////////////
        void program::uniform(const std::string& name, const basic_material& material)
        {
                glUniform3fv(findUniform(name + ".color"), 1, glm::value_ptr(material.mColor));
                glUniform1i(findUniform(name + ".materialTexture"), 0);
                glUniform1i(findUniform(name + ".enableTexture"), material.mEnableTexture);
                if (material.mEnableTexture && material.mBasicTexture)
                        material.mBasicTexture->bind();
        }

        ////////////////////////////////////////////////////////////////////////////////
        void program::uniform(const std::string& name, const phong_material& material)
        {
                glUniform3fv(findUniform(name + ".ambientColor"), 1, glm::value_ptr(material.mAmbient));
                glUniform3fv(findUniform(name + ".diffuseColor"), 1, glm::value_ptr(material.mDiffuse));
                glUniform3fv(findUniform(name + ".specularColor"), 1, glm::value_ptr(material.mSpecular));
                glUniform1f(findUniform(name + ".shininess"), material.mShininess);

                glUniform1i(findUniform(name + ".ambientTexture"), 0);
                glUniform1i(findUniform(name + ".enableAmbientTexture"), material.mEnableAmbientTexture);
                if (material.mEnableAmbientTexture && material.mAmbientTexture)
                        material.mAmbientTexture->bind(0);

                glUniform1i(findUniform(name + ".diffuseTexture"), 1);
                glUniform1i(findUniform(name + ".enableDiffuseTexture"), material.mEnableDiffuseTexture);
                if (material.mEnableDiffuseTexture && material.mDiffuseTexture)
                        material.mDiffuseTexture->bind(1);

                glUniform1i(findUniform(name + ".specularTexture"), 2);
                glUniform1i(findUniform(name + ".enableSpecularTexture"), material.mEnableSpecularTexture);
                if (material.mEnableSpecularTexture && material.mSpecularTexture)
                        material.mSpecularTexture->bind(2);
        }

        ////////////////////////////////////////////////////////////////////////////////
        void program::uniform(const std::string& name, const std::vector<dir_light>& lights)
        {
                int i = 0;
                for (const auto& light : lights) {
                        std::string s = name + "[" + std::to_string(i);
                        glUniform3fv(findUniform(s + "].ambient"),   1, glm::value_ptr(light.mAmbient));
                        glUniform3fv(findUniform(s + "].diffuse"),   1, glm::value_ptr(light.mDiffuse));
                        glUniform3fv(findUniform(s + "].specular"),  1, glm::value_ptr(light.mSpecular));
                        glUniform3fv(findUniform(s + "].direction"), 1, glm::value_ptr(light.mDirection));
                        glUniform1f(findUniform(s + "].intensity"),  light.mIntensity);
                        ++i;
                }
        }

        ////////////////////////////////////////////////////////////////////////////////
        void program::uniform(const std::string& name, const std::vector<point_light>& lights)
        {
                int i = 0;
                for (const auto& light : lights) {
                        std::string s = name + "[" + std::to_string(i);
                        glUniform3fv(findUniform(s + "].diffuse"),  1, glm::value_ptr(light.mDiffuse));
                        glUniform3fv(findUniform(s + "].specular"), 1, glm::value_ptr(light.mSpecular));
                        glUniform3fv(findUniform(s + "].position"), 1, glm::value_ptr(light.mPosition));
                        glUniform1f(findUniform(s + "].range"),     light.mRange);
                        glUniform1f(findUniform(s + "].intensity"), light.mIntensity);
                        glUniform1f(findUniform(s + "].constant"),  light.mConstant);
                        glUniform1f(findUniform(s + "].linear"),    light.mLinear);
                        glUniform1f(findUniform(s + "].quadratic"), light.mQuadratic);
                        ++i;
                }
        }

        ////////////////////////////////////////////////////////////////////////////////
        void program::uniform(const std::string& name, const std::vector<spot_light>& lights)
        {
                int i = 0;
                for (const auto& light : lights) {
                        std::string s = name + "[" + std::to_string(i);
                        glUniform3fv(findUniform(s + "].diffuse"),   1, glm::value_ptr(light.mDiffuse));
                        glUniform3fv(findUniform(s + "].specular"),  1, glm::value_ptr(light.mSpecular));
                        glUniform3fv(findUniform(s + "].position"),  1, glm::value_ptr(light.mPosition));
                        glUniform3fv(findUniform(s + "].direction"), 1, glm::value_ptr(light.mDirection));
                        glUniform1f(findUniform(s + "].range"),      light.mRange);
                        glUniform1f(findUniform(s + "].intensity"),  light.mIntensity);
                        glUniform1f(findUniform(s + "].cutoff"),     light.mCutoff);
                        glUniform1f(findUniform(s + "].outerCutoff"),light.mOuterCutoff);
                        glUniform1f(findUniform(s + "].constant"),   light.mConstant);
                        glUniform1f(findUniform(s + "].linear"),     light.mLinear);
                        glUniform1f(findUniform(s + "].quadratic"),  light.mQuadratic);
                        ++i;
                }
        }
}
