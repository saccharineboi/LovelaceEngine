#pragma once

#include <glm/vec3.hpp>

namespace al
{
        ////////////////////////////////////////////////////////////////////////////////
        struct dir_light
        {
                glm::vec3 mAmbient;
                glm::vec3 mDiffuse;
                glm::vec3 mSpecular;

                glm::vec3 mDirection;
                float mIntensity;
        };

        ////////////////////////////////////////////////////////////////////////////////
        struct point_light
        {
                glm::vec3 mDiffuse;
                glm::vec3 mSpecular;

                glm::vec3 mPosition;
                float mRange;
                float mIntensity;

                float mConstant;
                float mLinear;
                float mQuadratic;

                // source: https://wiki.ogre3d.org/Light+Attenuation+Shortcut
                void recalculateTerms(float constant = 1.0f)
                {
                        mConstant = constant;
                        mLinear = 4.5f / mRange;
                        mQuadratic = 75.0f / mRange;
                }
        };

        ////////////////////////////////////////////////////////////////////////////////
        struct spot_light
        {
                glm::vec3 mDiffuse;
                glm::vec3 mSpecular;

                glm::vec3 mPosition;
                glm::vec3 mDirection;
                float mRange;
                float mIntensity;

                float mCutoff;
                float mOuterCutoff;

                float mConstant;
                float mLinear;
                float mQuadratic;

                // source: https://wiki.ogre3d.org/Light+Attenuation+Shortcut
                void recalculateTerms(float constant = 1.0f)
                {
                        mConstant = constant;
                        mLinear = 4.5f / mRange;
                        mQuadratic = 75.0f / mRange;
                }
        };
}
