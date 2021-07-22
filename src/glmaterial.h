#pragma once

#include "glshader.h"
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
        struct basic_material
        {
                glm::vec4 mColor                        = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                bool mEnableTexture                     = false;
                texture2D* mBasicTexture                = nullptr;
        };

        ////////////////////////////////////////////////////////////////////////////////
        struct phong_material
        {
                glm::vec3 mAmbient                       = glm::vec3(1.0f, 1.0f, 1.0f);
                glm::vec3 mDiffuse                       = glm::vec3(1.0f, 1.0f, 1.0f);
                glm::vec3 mSpecular                      = glm::vec3(1.0f, 1.0f, 1.0f);
                float mShininess                         = 16.0f;

                bool mEnableAmbientTexture               = false;
                texture2D* mAmbientTexture               = nullptr;

                bool mEnableDiffuseTexture               = false;
                texture2D* mDiffuseTexture               = nullptr;

                bool mEnableSpecularTexture              = false;
                texture2D* mSpecularTexture              = nullptr;
        };
}
