#include <string>
#include <stb/stb_image.h>

#include "error.h"
#include "gltexture2D.h"

namespace al::gl
{
        ////////////////////////////////////////////////////////////////////////////////
        void texture2D::load()
        {
                glGenTextures(1, &mId);
                glBindTexture(GL_TEXTURE_2D, mId);
                        // default options
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mWrapS);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mWrapT);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mMinF);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mMagF);
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mData);
                        glGenerateMipmap(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, 0);
        }

        ////////////////////////////////////////////////////////////////////////////////
        texture2D::texture2D(const std::string& path)
                : mPath{path}
        {
                stbi_set_flip_vertically_on_load(true);

                int numDesiredChannels = 4;     // force stbi to load RGBA data
                mData = stbi_load(path.c_str(), &mWidth, &mHeight, &mNumChannels, numDesiredChannels);
                if (!mData)
                        throw exception("al::gl", "texture2D", "texture2D", path + " could not be loaded", etype::unexpected);
                load();
        }

        ////////////////////////////////////////////////////////////////////////////////
        texture2D::texture2D(const texture2D& other)
                : mWrapS{other.mWrapS}, mWrapT{other.mWrapT}, mMinF{other.mMinF}, mMagF{other.mMagF},
                  mWidth{other.mWidth}, mHeight{other.mHeight}, mNumChannels{other.mNumChannels}, mPath{other.mPath}
        {
                size_t size = mWidth * mHeight * mNumChannels;
                mData = new unsigned char[size];
                for (size_t i = 0; i < size; ++i)
                        mData[i] = other.mData[i];
                load();
        }

        ////////////////////////////////////////////////////////////////////////////////
        texture2D& texture2D::operator=(const texture2D& other)
        {
                mWrapS = other.mWrapS;
                mWrapT = other.mWrapT;
                mMinF = other.mMinF;
                mMagF = other.mMagF;

                mWidth = other.mWidth;
                mHeight = other.mHeight;
                mNumChannels = other.mNumChannels;
                mPath = other.mPath;

                delete[] mData;
                size_t size = mWidth * mHeight * mNumChannels;
                mData = new unsigned char[size];
                for (size_t i = 0; i < size; ++i)
                        mData[i] = other.mData[i];

                glDeleteTextures(1, &mId);
                load();
                return *this;
        }

        ////////////////////////////////////////////////////////////////////////////////
        texture2D::texture2D(texture2D&& other)
                : mId{other.mId}, mWrapS{other.mWrapS}, mWrapT{other.mWrapT}, mMinF{other.mMinF}, mMagF{other.mMagF},
                  mData{other.mData}, mWidth{other.mWidth}, mHeight{other.mHeight}, mNumChannels{other.mNumChannels},
                  mPath{other.mPath}
        {
                other.mId = other.mWidth = other.mHeight = other.mNumChannels = 0;
                other.mData = nullptr;
        }

        ////////////////////////////////////////////////////////////////////////////////
        texture2D& texture2D::operator=(texture2D&& other)
        {
                glDeleteTextures(1, &mId);
                delete[] mData;

                mId = other.mId;

                mWrapS = other.mWrapS;
                mWrapT = other.mWrapT;
                mMinF = other.mMinF;
                mMagF = other.mMagF;

                mData = other.mData;
                mWidth = other.mWidth;
                mHeight = other.mHeight;
                mNumChannels = other.mNumChannels;
                mPath = other.mPath;

                other.mId = other.mWidth = other.mHeight = other.mNumChannels = 0;
                other.mData = nullptr;
                return *this;
        }

        ////////////////////////////////////////////////////////////////////////////////
        texture2D::~texture2D()
        {
                glDeleteTextures(1, &mId);
                if (mData)
                        stbi_image_free(mData);
        }
}
