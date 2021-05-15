#pragma once

#include <glad/glad.h>
#include <vector>

#include "glbuffer.h"
#include "glutils.h"

namespace al::gl
{
        ////////////////////////////////////////////////////////////////////////////////
        struct vao_info
        {
                unsigned index;
                int size;
                int type;
                int normalized;
                int stride;
                void* offset;
        };

        ////////////////////////////////////////////////////////////////////////////////
        template <typename VT, typename IT>
        class vao
        {
                unsigned mId;
                unsigned mVboId;
                unsigned mEboId;
                size_t mNumElements;

                std::vector<vao_info> mInfos;

                void load();
        public:
                vao(const buffer<VT>& vbo, const buffer<IT>& ebo, const std::vector<vao_info>& infos);

                ~vao()                          { glDeleteVertexArrays(1, &mId); }

                vao(const vao&)                 = delete;
                vao& operator=(const vao&)      = delete;

                vao(vao&&);
                vao& operator=(vao&&);

                unsigned getId() const          { return mId; }
                unsigned getVboId() const       { return mVboId; }
                unsigned getEboId() const       { return mEboId; }
                size_t getNumElements() const   { return mNumElements; }

                void bind() const               { glBindVertexArray(mId); }
                void unbind() const             { glBindVertexArray(0); }

                void draw(int mode = GL_TRIANGLES) const { glDrawElements(mode, (GLsizei)mNumElements, utils::findEboType<IT>(), nullptr); }
        };

        ////////////////////////////////////////////////////////////////////////////////
        template <typename VT, typename IT>
        void vao<VT, IT>::load()
        {
                glGenVertexArrays(1, &mId);
                glBindVertexArray(mId);
                        glBindBuffer(GL_ARRAY_BUFFER, mVboId);
                        for (const vao_info& info : mInfos) {
                                glVertexAttribPointer(info.index, info.size, info.type, info.normalized, info.stride, info.offset);
                                glEnableVertexAttribArray(info.index);
                        }
                        glBindBuffer(GL_ARRAY_BUFFER, 0);
                        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEboId);
                glBindVertexArray(0);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

        ////////////////////////////////////////////////////////////////////////////////
        template <typename VT, typename IT>
        vao<VT, IT>::vao(const buffer<VT>& vbo, const buffer<IT>& ebo, const std::vector<vao_info>& infos)
                : mVboId{vbo.getId()}, mEboId{ebo.getId()}, mNumElements{ebo.getSize()}, mInfos{infos}
        {
                load();
        }

        ////////////////////////////////////////////////////////////////////////////////
        template <typename VT, typename IT>
        vao<VT, IT>::vao(vao<VT, IT>&& other)
                : mId{other.mId}, mVboId{other.mVboId}, mEboId{other.mEboId}, mNumElements{other.mNumElements}, mInfos{std::move(other.mInfos)}
        {
                other.mId = 0;
        }

        ////////////////////////////////////////////////////////////////////////////////
        template <typename VT, typename IT>
        vao<VT, IT>& vao<VT, IT>::operator=(vao<VT, IT>&& other)
        {
                if (this != &other) {
                        glDeleteVertexArrays(1, &mId);

                        mId             = other.mId;
                        mVboId          = other.mVboId;
                        mEboId          = other.mEboId;
                        mNumElements    = other.mNumElements;
                        mInfos          = std::move(other.mInfos);

                        other.mId       = 0;
                }
                return *this;
        }

        ////////////////////////////////////////////////////////////////////////////////
        template <typename VT, typename IT>
        bool operator==(const vao<VT, IT>& first, const vao<VT, IT>& second) { return first.getId() == second.getId(); }

        ////////////////////////////////////////////////////////////////////////////////
        template <typename VT, typename IT>
        bool operator!=(const vao<VT, IT>& first, const vao<VT, IT>& second) { return first.getId() != second.getId(); }
}

