#include "fpscamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define LERP(RES, START, END, DELTA) ((RES) = (START) + (DELTA) * ((END) - (START)))

namespace al
{
        ////////////////////////////////////////////////////////////////////////////////
        fpscamera::fpscamera(float screenWidth, float screenHeight, const fpscamera_input& input)
                : mScreenWidth{screenWidth}, mScreenHeight{screenHeight}, mInput{input}
        {
                mProjection = glm::perspective(glm::radians(mFov), screenWidth / screenHeight, mNear, mFar);
                mView = glm::lookAt(mPosition, mPosition + mDirection, mUp);
        }

        ////////////////////////////////////////////////////////////////////////////////
        void fpscamera::updateState(int key, int action)
        {
                if (!mActive)
                        return;

                if (key == mInput.forwardButton) {
                        if (action == mInput.pressButton) {
                                mState.moveForward = true;
                                mState.moveBackward = false;
                        }
                        else if (action == mInput.releaseButton)
                                mState.moveForward = false;
                }
                else if (key == mInput.backwardButton) {
                        if (action == mInput.pressButton) {
                                mState.moveBackward = true;
                                mState.moveForward = false;
                        }
                        else if (action == mInput.releaseButton)
                                mState.moveBackward = false;
                }

                if (key == mInput.leftButton) {
                        if (action == mInput.pressButton) {
                                mState.moveLeft = true;
                                mState.moveRight = false;
                        }
                        else if (action == mInput.releaseButton)
                                mState.moveLeft = false;
                }
                else if (key == mInput.rightButton) {
                        if (action == mInput.pressButton) {
                                mState.moveRight = true;
                                mState.moveLeft = false;
                        }
                        else if (action == mInput.releaseButton)
                                mState.moveRight = false;
                }

                if (key == mInput.upButton) {
                        if (action == mInput.pressButton) {
                                mState.moveUp = true;
                                mState.moveDown = false;
                        }
                        else if (action == mInput.releaseButton)
                                mState.moveUp = false;
                }
                else if (key == mInput.downButton) {
                        if (action == mInput.pressButton) {
                                mState.moveDown = true;
                                mState.moveUp = false;
                        }
                        else if (action == mInput.releaseButton)
                                mState.moveDown = false;
                }

                if (key == mInput.fasterButton) {
                        if (action == mInput.pressButton)
                                mState.moveFaster = true;
                        else if (action == mInput.releaseButton)
                                mState.moveFaster = false;
                }
        }

        ////////////////////////////////////////////////////////////////////////////////
        void fpscamera::orient(float xpos, float ypos)
        {
                if (mFirstTime) {
                        mFirstTime = false;
                        mLastX = xpos;
                        mLastY = ypos;
                }

                if (!mActive) {
                        mLastX = xpos;
                        mLastY = ypos;
                        return;
                }

                float xoffset = xpos - mLastX;
                float yoffset = mLastY - ypos;

                mLastX = xpos;
                mLastY = ypos;

                xoffset *= mSensitivity;
                yoffset *= mSensitivity;

                float pitch = mOrientation.x;
                float yaw = mOrientation.y;

                yaw += xoffset;
                pitch += yoffset;

                if (pitch > mVerticalLimit)
                        pitch = mVerticalLimit;
                else if (pitch < -mVerticalLimit)
                        pitch = -mVerticalLimit;

                float pitch_sin = glm::sin(glm::radians(pitch));
                float pitch_cos = glm::cos(glm::radians(pitch));
                float yaw_sin = glm::sin(glm::radians(yaw));
                float yaw_cos = glm::cos(glm::radians(yaw));

                mDirection = glm::vec3(yaw_cos * pitch_cos, pitch_sin, yaw_sin * pitch_cos);

                mOrientation.x = pitch;
                mOrientation.y = yaw;
        }

        ////////////////////////////////////////////////////////////////////////////////
        void fpscamera::update(float deltaTime)
        {
                if (!mActive)
                        return;

                float newSpeed = mState.moveFaster ? mSpeedMultiplier * mSpeed : mSpeed;

                if (mState.moveForward)
                        mNewPos += mDirection * newSpeed * deltaTime;
                else if (mState.moveBackward)
                        mNewPos -= mDirection * newSpeed * deltaTime;
                if (mState.moveLeft)
                        mNewPos -= glm::normalize(glm::cross(mDirection, mUp)) * newSpeed * deltaTime;
                else if (mState.moveRight)
                        mNewPos += glm::normalize(glm::cross(mDirection, mUp)) * newSpeed * deltaTime;
                if (mState.moveUp)
                        mNewPos += mUp * newSpeed * deltaTime;
                else if (mState.moveDown)
                        mNewPos -= mUp * newSpeed * deltaTime;

                LERP(mPosition, mPosition, mNewPos, mSmoothness);

                mProjection = glm::perspective(glm::radians(mFov), mScreenWidth / mScreenHeight, mNear, mFar);
                mView = glm::lookAt(mPosition, mPosition + mDirection, mUp);
        }
}
