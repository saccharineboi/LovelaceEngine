#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace al
{
        ////////////////////////////////////////////////////////////////////////////////
        struct fpscamera_state
        {
                bool moveForward                = false;
                bool moveBackward               = false;
                bool moveLeft                   = false;
                bool moveRight                  = false;
                bool moveUp                     = false;
                bool moveDown                   = false;
                bool moveFaster                 = false;
        };

        ////////////////////////////////////////////////////////////////////////////////
        struct fpscamera_input
        {
                int forwardButton;
                int backwardButton;
                int leftButton;
                int rightButton;
                int upButton;
                int downButton;
                int fasterButton;
                int pressButton;
                int releaseButton;
        };

        ////////////////////////////////////////////////////////////////////////////////
        class fpscamera
        {
                glm::mat4 mProjection;
                glm::mat4 mView;

        public:
                float mScreenWidth, mScreenHeight;

                fpscamera_input mInput;
                fpscamera_state mState;
                
                glm::vec3 mPosition             = glm::vec3(0.0f, 5.0f, 10.0f);
                glm::vec3 mDirection            = glm::vec3(0.0f, 0.0f, -1.0f);
                glm::vec3 mUp                   = glm::vec3(0.0f, 1.0f, 0.0f);
                glm::vec3 mOrientation          = glm::vec3(0.0f, -90.0f, 0.0f);

                float mSpeed                    = 10.0f;
                float mSpeedMultiplier          = 2.0f;
                float mSensitivity              = 0.1f;
                float mSmoothness               = 0.1f;
                float mFov                      = 90.0f;
                float mVerticalLimit            = 89.0f;
                float mNear                     = 0.1f;
                float mFar                      = 1000.0f;

                bool mActive                    = false;

                fpscamera(float screenWidth, float screenHeight, const fpscamera_input& input);

                void orient(float xpos, float ypos);
                void updateState(int key, int action);
                void update(float deltaTime);

                glm::mat4 getProjection() const                 { return mProjection; }
                glm::mat4 getView() const                       { return mView; }
        };
}
