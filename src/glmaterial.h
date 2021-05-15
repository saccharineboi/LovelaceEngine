#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <string>
#include <vector>

#include <cassert>

#include "glshader.h"
#include "glprogram.h"
#include "gltexture2D.h"
#include "lights.h"

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
                abstract_material(int vshader_url_index, int fshader_url_index);
                ~abstract_material() {}

                void use() const                        { mProgram.use(); }
                void halt() const                       { mProgram.halt(); }

                virtual void update()                   = 0;
        };

        ////////////////////////////////////////////////////////////////////////////////
        struct basic_color_material final : public abstract_material
        {
                glm::vec4 mColor                        = glm::vec4(1.0f, 0.5f, 0.25f, 1.0f);
                glm::mat4* mPVM                         = nullptr;

                basic_color_material();

                virtual void update() override
                {
                        using namespace std::string_literals;

                        assert(mPVM != nullptr);
                        mProgram.uniform("uPVM"s, *mPVM);
                        mProgram.uniform("uColor"s, mColor);
                }
        };

        ////////////////////////////////////////////////////////////////////////////////
        struct basic_color_interpolated_material final : public abstract_material
        {
                glm::mat4* mPVM                         = nullptr;

                basic_color_interpolated_material();

                virtual void update() override
                {
                        using namespace std::string_literals;

                        assert(mPVM != nullptr);
                        mProgram.uniform("uPVM"s, *mPVM);
                }
        };

        ////////////////////////////////////////////////////////////////////////////////
        struct basic_texture_material final : public abstract_material
        {
                glm::mat4* mPVM                         = nullptr;

                basic_texture_material();

                virtual void update() override
                {
                        using namespace std::string_literals;

                        assert(mPVM != nullptr);
                        mProgram.uniform("uPVM"s, *mPVM);
                }
        };

        ////////////////////////////////////////////////////////////////////////////////
        struct basic_texture_interpolated_material final : public abstract_material
        {
                glm::mat4* mPVM                         = nullptr;
                float mInterpolation                    = 0.5f;

                basic_texture_interpolated_material();

                virtual void update() override
                {
                        using namespace std::string_literals;

                        assert(mPVM != nullptr);
                        mProgram.uniform("uInterpolation"s, mInterpolation);
                        mProgram.uniform("uPVM"s, *mPVM);
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

                glm::mat4* mPVM                          = nullptr;
                glm::mat4* mModel                        = nullptr;
                glm::mat4* mNormal                       = nullptr;
                glm::vec3* mViewPos                      = nullptr;

                phong_material();

                virtual void update() override
                {
                        using namespace std::string_literals;

                        assert(mPVM != nullptr && mModel != nullptr && mNormal != nullptr && mViewPos != nullptr);

                        mProgram.uniform("uPVM"s, *mPVM);
                        mProgram.uniform("uModel"s, *mModel);
                        mProgram.uniform("uNormal"s, *mNormal);
                        mProgram.uniform("uViewPos"s, *mViewPos);

                        mProgram.uniform("uMaterial.ambient"s, mAmbient);
                        mProgram.uniform("uMaterial.diffuse"s, mDiffuse);
                        mProgram.uniform("uMaterial.specular"s, mSpecular);
                        mProgram.uniform("uMaterial.shininess"s, mShininess);

                        for (size_t i = 0; i < mDirLights.size() && i < NUM_DIR_LIGHTS; ++i) {
                                assert(mDirLights[i] != nullptr);

                                std::string s = "uDirLights["s + std::to_string(i);
                                mProgram.uniform(s + "].ambient"s,              mDirLights[i]->mAmbient);
                                mProgram.uniform(s + "].diffuse"s,              mDirLights[i]->mDiffuse);
                                mProgram.uniform(s + "].specular"s,             mDirLights[i]->mSpecular);
                                mProgram.uniform(s + "].direction"s,            mDirLights[i]->mDirection);
                                mProgram.uniform(s + "].intensity"s,            mDirLights[i]->mIntensity);
                        }

                        for (size_t i = 0; i < mPointLights.size() && i < NUM_POINT_LIGHTS; ++i) {
                                assert(mPointLights[i] != nullptr);

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
                                assert(mSpotLights[i] != nullptr);

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

                glm::mat4* mPVM                          = nullptr;
                glm::mat4* mModel                        = nullptr;
                glm::mat4* mNormal                       = nullptr;
                glm::vec3* mViewPos                      = nullptr;

                glm::vec2 mTextureTile                   = glm::vec2(1.0f);
                glm::vec2 mTextureOffset                 = glm::vec2(0.0f);

                phong_textured_material();

                virtual void update() override
                {
                        using namespace std::string_literals;

                        assert(mPVM != nullptr && mModel != nullptr && mNormal != nullptr && mViewPos != nullptr);

                        mProgram.uniform("uPVM"s, *mPVM);
                        mProgram.uniform("uModel"s, *mModel);
                        mProgram.uniform("uNormal"s, *mNormal);
                        mProgram.uniform("uViewPos"s, *mViewPos);

                        mProgram.uniform("uTextureTile"s, mTextureTile);
                        mProgram.uniform("uTextureOffset"s, mTextureOffset);

                        mProgram.uniform("uMaterial.shininess"s, mShininess);

                        for (size_t i = 0; i < mDirLights.size() && i < NUM_DIR_LIGHTS; ++i) {
                                assert(mDirLights[i] != nullptr);

                                std::string s = "uDirLights["s + std::to_string(i);
                                mProgram.uniform(s + "].ambient"s,              mDirLights[i]->mAmbient);
                                mProgram.uniform(s + "].diffuse"s,              mDirLights[i]->mDiffuse);
                                mProgram.uniform(s + "].specular"s,             mDirLights[i]->mSpecular);
                                mProgram.uniform(s + "].direction"s,            mDirLights[i]->mDirection);
                                mProgram.uniform(s + "].intensity"s,            mDirLights[i]->mIntensity);
                        }

                        for (size_t i = 0; i < mPointLights.size() && i < NUM_POINT_LIGHTS; ++i) {
                                assert(mPointLights[i] != nullptr);

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
                                assert(mSpotLights[i] != nullptr);

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
        struct gouraud_material final : public abstract_material
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

                glm::mat4* mPVM                          = nullptr;
                glm::mat4* mModel                        = nullptr;
                glm::mat4* mNormal                       = nullptr;
                glm::vec3* mViewPos                      = nullptr;

                gouraud_material();

                virtual void update() override
                {
                        using namespace std::string_literals;

                        assert(mPVM != nullptr && mModel != nullptr && mNormal != nullptr && mViewPos != nullptr);

                        mProgram.uniform("uPVM"s, *mPVM);
                        mProgram.uniform("uModel"s, *mModel);
                        mProgram.uniform("uNormal"s, *mNormal);
                        mProgram.uniform("uViewPos"s, *mViewPos);

                        mProgram.uniform("uMaterial.ambient"s, mAmbient);
                        mProgram.uniform("uMaterial.diffuse"s, mDiffuse);
                        mProgram.uniform("uMaterial.specular"s, mSpecular);
                        mProgram.uniform("uMaterial.shininess"s, mShininess);

                        for (size_t i = 0; i < mDirLights.size() && i < NUM_DIR_LIGHTS; ++i) {
                                assert(mDirLights[i] != nullptr);

                                std::string s = "uDirLights["s + std::to_string(i);
                                mProgram.uniform(s + "].ambient"s,              mDirLights[i]->mAmbient);
                                mProgram.uniform(s + "].diffuse"s,              mDirLights[i]->mDiffuse);
                                mProgram.uniform(s + "].specular"s,             mDirLights[i]->mSpecular);
                                mProgram.uniform(s + "].direction"s,            mDirLights[i]->mDirection);
                                mProgram.uniform(s + "].intensity"s,            mDirLights[i]->mIntensity);
                        }

                        for (size_t i = 0; i < mPointLights.size() && i < NUM_POINT_LIGHTS; ++i) {
                                assert(mPointLights[i] != nullptr);

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
                                assert(mSpotLights[i] != nullptr);

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
