#pragma once

#include "glshader.h"
#include "glprogram.h"
#include "gltexture2D.h"
#include "lights.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <string>
#include <vector>

namespace al::gl
{
        ////////////////////////////////////////////////////////////////////////////////
        class abstract_material
        {
        protected:
                shader* mVshader                        = nullptr;
                shader* mFshader                        = nullptr;
                program mProgram;

        public:
                explicit abstract_material(int shader_url_index);
                ~abstract_material() {}

                void use() const                        { mProgram.use(); }
                void halt() const                       { mProgram.halt(); }
        };

        ////////////////////////////////////////////////////////////////////////////////
        struct basic_color_material final : public abstract_material
        {
                basic_color_material();

                void update(const glm::vec4& color, const glm::mat4& pvm)
                {
                        using namespace std::string_literals;
                        mProgram.uniform("uColor"s, color);
                        mProgram.uniform("uPVM"s, pvm);
                }
        };

        ////////////////////////////////////////////////////////////////////////////////
        struct basic_color_interpolated_material final : public abstract_material
        {
                basic_color_interpolated_material();

                void update(const glm::mat4& pvm)
                {
                        using namespace std::string_literals;
                        mProgram.uniform("uPVM"s, pvm);
                }
        };

        ////////////////////////////////////////////////////////////////////////////////
        struct basic_texture_material final : public abstract_material
        {
                basic_texture_material();

                void update(const glm::mat4& pvm)
                {
                        using namespace std::string_literals;
                        mProgram.uniform("uPVM"s, pvm);
                }
        };

        ////////////////////////////////////////////////////////////////////////////////
        struct basic_texture_interpolated_material final : public abstract_material
        {
                basic_texture_interpolated_material();

                void update(float interpolation, const glm::mat4& pvm)
                {
                        using namespace std::string_literals;
                        mProgram.uniform("uInterpolation"s, interpolation);
                        mProgram.uniform("uPVM"s, pvm);
                }
        };

        ////////////////////////////////////////////////////////////////////////////////
        struct phong_material final : public abstract_material
        {
                static constexpr int NUM_DIR_LIGHTS      = 2;
                static constexpr int NUM_POINT_LIGHTS    = 4;
                static constexpr int NUM_SPOT_LIGHTS     = 2;

                std::vector<dir_light*> mDirLights;
                std::vector<point_light*> mPointLights;
                std::vector<spot_light*> mSpotLights;

                glm::vec3 mAmbient                       = glm::vec3(1.0f, 1.0f, 1.0f);
                glm::vec3 mDiffuse                       = glm::vec3(1.0f, 1.0f, 1.0f);
                glm::vec3 mSpecular                      = glm::vec3(1.0f, 1.0f, 1.0f);
                float mShininess                         = 16.0f;

                phong_material();

                void update(const glm::vec3& viewPos, const glm::mat4& normal, const glm::mat4& model, const glm::mat4& pvm)
                {
                        using namespace std::string_literals;

                        mProgram.uniform("uViewPos"s, viewPos);
                        mProgram.uniform("uNormal"s, normal);
                        mProgram.uniform("uModel"s, model);
                        mProgram.uniform("uPVM"s, pvm);

                        mProgram.uniform("uMaterial.ambient"s, mAmbient);
                        mProgram.uniform("uMaterial.diffuse"s, mDiffuse);
                        mProgram.uniform("uMaterial.specular"s, mSpecular);
                        mProgram.uniform("uMaterial.shininess"s, mShininess);

                        for (size_t i = 0; i < mDirLights.size() && i < NUM_DIR_LIGHTS; ++i) {
                                std::string s = "uDirLights["s + std::to_string(i);
                                mProgram.uniform(s + "].ambient"s,              mDirLights[i]->mAmbient);
                                mProgram.uniform(s + "].diffuse"s,              mDirLights[i]->mDiffuse);
                                mProgram.uniform(s + "].specular"s,             mDirLights[i]->mSpecular);
                                mProgram.uniform(s + "].direction"s,            mDirLights[i]->mDirection);
                                mProgram.uniform(s + "].intensity"s,            mDirLights[i]->mIntensity);
                        }

                        for (size_t i = 0; i < mPointLights.size() && i < NUM_POINT_LIGHTS; ++i) {
                                std::string s = "uPointLights["s + std::to_string(i);
                                mProgram.uniform(s + "].diffuse"s,              mPointLights[i]->mDiffuse);
                                mProgram.uniform(s + "].specular"s,             mPointLights[i]->mSpecular);
                                mProgram.uniform(s + "].position"s,             mPointLights[i]->mPosition);
                                mProgram.uniform(s + "].range"s,                mPointLights[i]->mRange);
                                mProgram.uniform(s + "].intensity"s,            mPointLights[i]->mIntensity);
                                mProgram.uniform(s + "].constant"s,             mPointLights[i]->mConstant);
                                mProgram.uniform(s + "].linear"s,               mPointLights[i]->mLinear);
                                mProgram.uniform(s + "].quadratic"s,            mPointLights[i]->mQuadratic);
                        }

                        for (size_t i = 0; i < mSpotLights.size() && i < NUM_SPOT_LIGHTS; ++i) {
                                std::string s = "uSpotLights["s + std::to_string(i);
                                mProgram.uniform(s + "].diffuse"s,              mSpotLights[i]->mDiffuse);
                                mProgram.uniform(s + "].specular"s,             mSpotLights[i]->mSpecular);
                                mProgram.uniform(s + "].position"s,             mSpotLights[i]->mPosition);
                                mProgram.uniform(s + "].direction"s,            mSpotLights[i]->mDirection);
                                mProgram.uniform(s + "].range"s,                mSpotLights[i]->mRange);
                                mProgram.uniform(s + "].intensity"s,            mSpotLights[i]->mIntensity);
                                mProgram.uniform(s + "].cutoff"s,               mSpotLights[i]->mCutoff);
                                mProgram.uniform(s + "].outerCutoff"s,          mSpotLights[i]->mOuterCutoff);
                                mProgram.uniform(s + "].constant"s,             mSpotLights[i]->mConstant);
                                mProgram.uniform(s + "].linear"s,               mSpotLights[i]->mLinear);
                                mProgram.uniform(s + "].quadratic"s,            mSpotLights[i]->mQuadratic);
                        }
                }
        };

        ////////////////////////////////////////////////////////////////////////////////
        struct phong_textured_material final : public abstract_material
        {
                static constexpr int NUM_DIR_LIGHTS      = 2;
                static constexpr int NUM_POINT_LIGHTS    = 4;
                static constexpr int NUM_SPOT_LIGHTS     = 2;

                std::vector<dir_light*> mDirLights;
                std::vector<point_light*> mPointLights;
                std::vector<spot_light*> mSpotLights;

                float mShininess                         = 16.0f;

                glm::vec2 mTextureTile                   = glm::vec2(1.0f);
                glm::vec2 mTextureOffset                 = glm::vec2(0.0f);

                phong_textured_material();

                void update(const glm::vec3& viewPos, const glm::mat4& normal, const glm::mat4& model, const glm::mat4& pvm)
                {
                        using namespace std::string_literals;

                        mProgram.uniform("uViewPos"s, viewPos);
                        mProgram.uniform("uNormal"s, normal);
                        mProgram.uniform("uModel"s, model);
                        mProgram.uniform("uPVM"s, pvm);

                        mProgram.uniform("uTextureTile"s, mTextureTile);
                        mProgram.uniform("uTextureOffset"s, mTextureOffset);

                        mProgram.uniform("uMaterial.shininess"s, mShininess);

                        for (size_t i = 0; i < mDirLights.size() && i < NUM_DIR_LIGHTS; ++i) {
                                std::string s = "uDirLights["s + std::to_string(i);
                                mProgram.uniform(s + "].ambient"s,              mDirLights[i]->mAmbient);
                                mProgram.uniform(s + "].diffuse"s,              mDirLights[i]->mDiffuse);
                                mProgram.uniform(s + "].specular"s,             mDirLights[i]->mSpecular);
                                mProgram.uniform(s + "].direction"s,            mDirLights[i]->mDirection);
                                mProgram.uniform(s + "].intensity"s,            mDirLights[i]->mIntensity);
                        }

                        for (size_t i = 0; i < mPointLights.size() && i < NUM_POINT_LIGHTS; ++i) {
                                std::string s = "uPointLights["s + std::to_string(i);
                                mProgram.uniform(s + "].diffuse"s,              mPointLights[i]->mDiffuse);
                                mProgram.uniform(s + "].specular"s,             mPointLights[i]->mSpecular);
                                mProgram.uniform(s + "].position"s,             mPointLights[i]->mPosition);
                                mProgram.uniform(s + "].range"s,                mPointLights[i]->mRange);
                                mProgram.uniform(s + "].intensity"s,            mPointLights[i]->mIntensity);
                                mProgram.uniform(s + "].constant"s,             mPointLights[i]->mConstant);
                                mProgram.uniform(s + "].linear"s,               mPointLights[i]->mLinear);
                                mProgram.uniform(s + "].quadratic"s,            mPointLights[i]->mQuadratic);
                        }

                        for (size_t i = 0; i < mSpotLights.size() && i < NUM_SPOT_LIGHTS; ++i) {
                                std::string s = "uSpotLights["s + std::to_string(i);
                                mProgram.uniform(s + "].diffuse"s,              mSpotLights[i]->mDiffuse);
                                mProgram.uniform(s + "].specular"s,             mSpotLights[i]->mSpecular);
                                mProgram.uniform(s + "].position"s,             mSpotLights[i]->mPosition);
                                mProgram.uniform(s + "].direction"s,            mSpotLights[i]->mDirection);
                                mProgram.uniform(s + "].range"s,                mSpotLights[i]->mRange);
                                mProgram.uniform(s + "].intensity"s,            mSpotLights[i]->mIntensity);
                                mProgram.uniform(s + "].cutoff"s,               mSpotLights[i]->mCutoff);
                                mProgram.uniform(s + "].outerCutoff"s,          mSpotLights[i]->mOuterCutoff);
                                mProgram.uniform(s + "].constant"s,             mSpotLights[i]->mConstant);
                                mProgram.uniform(s + "].linear"s,               mSpotLights[i]->mLinear);
                                mProgram.uniform(s + "].quadratic"s,            mSpotLights[i]->mQuadratic);
                        }
                }
        };
}
