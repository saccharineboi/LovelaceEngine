#pragma once

#include <string>
#include <glad/glad.h>

namespace al::gl
{
        ////////////////////////////////////////////////////////////////////////////////
        class texture2D
        {
                unsigned mId;

                // default options
                int mWrapS      = GL_REPEAT;
                int mWrapT      = GL_REPEAT;
                int mMinF       = GL_LINEAR_MIPMAP_LINEAR;
                int mMagF       = GL_LINEAR;

                unsigned char* mData = nullptr;
                int mWidth, mHeight;
                int mNumChannels;

                std::string mPath;

                void load();

        public:
                explicit texture2D(const std::string& path);

                ~texture2D();

                texture2D(const texture2D&);
                texture2D& operator=(const texture2D&);

                texture2D(texture2D&&);
                texture2D& operator=(texture2D&&);

                void bind(int i = 0) const      { glActiveTexture(GL_TEXTURE0 + i); glBindTexture(GL_TEXTURE_2D, mId); }
                void unbind(int i = 0) const    { glActiveTexture(GL_TEXTURE0 + i); glBindTexture(GL_TEXTURE_2D, 0); }

                unsigned getId() const          { return mId; }
                int getWrapS() const            { return mWrapS; }
                int getWrapT() const            { return mWrapT; }
                int getMinF() const             { return mMinF; }
                int getMagF() const             { return mMagF; }

                int getWidth() const            { return mWidth; }
                int getHeight() const           { return mHeight; }
                int getNumChannels() const      { return mNumChannels; }
                int getSizeInBytes() const      { return mWidth * mHeight * mNumChannels; }

                std::string getPath() const     { return mPath; }

                void setWrapS(int wrapS)        { mWrapS = wrapS; }
                void setWrapT(int wrapT)        { mWrapT = wrapT; }
                void setMinF(int minF)          { mMinF = minF; }
                void setMagF(int magF)          { mMagF = magF; }
        };

        ////////////////////////////////////////////////////////////////////////////////
        inline bool operator==(const texture2D& first, const texture2D& second) { return first.getId() == second.getId(); }

        ////////////////////////////////////////////////////////////////////////////////
        inline bool operator!=(const texture2D& first, const texture2D& second) { return first.getId() != second.getId(); }
}
