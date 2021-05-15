#pragma once

#include <glad/glad.h>
#include <vector>

namespace al::gl
{
        ////////////////////////////////////////////////////////////////////////////////
        template <typename T>
        class buffer
        {
                unsigned mId;
                int mMode;
                std::vector<T> mData;
                int mUsage;

                void load();
        public:
                buffer(int mode, std::vector<T>&& data, int usage);

                ~buffer()                               { glDeleteBuffers(1, &mId); }

                buffer(const buffer&);
                buffer& operator=(const buffer&);

                buffer(buffer&&);
                buffer& operator=(buffer&&);

                unsigned getId() const                  { return mId; }
                int getMode() const                     { return mMode; }
                size_t getSize() const                  { return mData.size(); }
                size_t getSizeInBytes() const           { return mData.size() * sizeof(T); }
                int getUsage() const                    { return mUsage; }

                void bind() const                       { glBindBuffer(mMode, mId); }
                void unbind() const                     { glBindBuffer(mMode, 0); }
        };

        ////////////////////////////////////////////////////////////////////////////////
        template <typename T>
        void buffer<T>::load()
        {
                glGenBuffers(1, &mId);
                glBindBuffer(mMode, mId);
                        size_t bytes = mData.size() * sizeof(T);
                        glBufferData(mMode, bytes, mData.data(), mUsage);
                glBindBuffer(mMode, 0);
        }

        ////////////////////////////////////////////////////////////////////////////////
        template <typename T>
        buffer<T>::buffer(int mode, std::vector<T>&& data, int usage)
                : mMode{mode}, mData{std::move(data)}, mUsage{usage}
        {
                load();
        }

        ////////////////////////////////////////////////////////////////////////////////
        template <typename T>
        buffer<T>::buffer(const buffer<T>& other)
                : mMode{other.mMode}, mData{other.mData}, mUsage{other.mUsage}
        {
                load();
        }

        ////////////////////////////////////////////////////////////////////////////////
        template <typename T>
        buffer<T>& buffer<T>::operator=(const buffer<T>& other)
        {
                if (this != &other) {
                        glDeleteBuffers(1, &mId);

                        mMode   = other.mMode;
                        mData   = other.mData;
                        mUsage  = other.mUsage;

                        load();
                }
                return *this;
        }

        ////////////////////////////////////////////////////////////////////////////////
        template <typename T>
        buffer<T>::buffer(buffer<T>&& other)
                : mId{other.mId}, mMode{other.mMode}, mData{std::move(other.mData)}, mUsage{other.mUsage}
        {
                other.mId = 0;
        }

        ////////////////////////////////////////////////////////////////////////////////
        template <typename T>
        buffer<T>& buffer<T>::operator=(buffer<T>&& other)
        {
                if (this != &other) {
                        glDeleteBuffers(1, &mId);

                        mId     = other.mId;
                        mMode   = other.mMode;
                        mData   = std::move(other.mData);
                        mUsage  = other.mUsage;

                        other.mId = 0;
                }
                return *this;
        }

        ////////////////////////////////////////////////////////////////////////////////
        template <typename T>
        inline bool operator==(const buffer<T>& first, const buffer<T>& second) { return first.getId() == second.getId(); }

        ////////////////////////////////////////////////////////////////////////////////
        template <typename T>
        inline bool operator!=(const buffer<T>& first, const buffer<T>& second) { return first.getId() != second.getId(); }
}
