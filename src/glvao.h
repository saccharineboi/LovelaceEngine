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
                buffer<VT> mVbo;
                buffer<IT> mEbo;

                std::vector<vao_info> mInfos;

                void load();
        public:
                vao(buffer<VT>&& vbo, buffer<IT>&& ebo, const std::vector<vao_info>& infos);

                ~vao()                          { glDeleteVertexArrays(1, &mId); }

                vao(const vao&);
                vao& operator=(const vao&);

                vao(vao&&);
                vao& operator=(vao&&);

                unsigned getId() const          { return mId; }

                void bind() const               { glBindVertexArray(mId); }
                void unbind() const             { glBindVertexArray(0); }

                void draw(int mode = GL_TRIANGLES) const { glDrawElements(mode, mEbo.getSize(), utils::findEboType<IT>(), nullptr); }
        };

        ////////////////////////////////////////////////////////////////////////////////
        template <typename VT, typename IT>
        void vao<VT, IT>::load()
        {
                glGenVertexArrays(1, &mId);
                glBindVertexArray(mId);
                        mVbo.bind();
                        for (const vao_info& info : mInfos) {
                                glVertexAttribPointer(info.index, info.size, info.type, info.normalized, info.stride, info.offset);
                                glEnableVertexAttribArray(info.index);
                        }
                        mVbo.unbind();
                        mEbo.bind();
                glBindVertexArray(0);
                mEbo.unbind();
        }

        ////////////////////////////////////////////////////////////////////////////////
        template <typename VT, typename IT>
        vao<VT, IT>::vao(buffer<VT>&& vbo, buffer<IT>&& ebo, const std::vector<vao_info>& infos)
                : mVbo{std::move(vbo)}, mEbo{std::move(ebo)}, mInfos{infos}
        {
                load();
        }

        ////////////////////////////////////////////////////////////////////////////////
        template <typename VT, typename IT>
        vao<VT, IT>::vao(const vao<VT, IT>& other)
                : mVbo{other.mVbo}, mEbo{other.mEbo}, mInfos{other.mInfos}
        {
                load();
        }

        ////////////////////////////////////////////////////////////////////////////////
        template <typename VT, typename IT>
        vao<VT, IT>& vao<VT, IT>::operator=(const vao<VT, IT>& other)
        {
                if (this != &other) {
                        glDeleteVertexArrays(1, &mId);

                        mVbo    = other.mVbo;
                        mEbo    = other.mEbo;
                        mInfos  = other.mInfos;

                        load();
                }
                return *this;
        }

        ////////////////////////////////////////////////////////////////////////////////
        template <typename VT, typename IT>
        vao<VT, IT>::vao(vao<VT, IT>&& other)
                : mId{other.mId}, mVbo{std::move(other.mVbo)}, mEbo{std::move(other.mEbo)}, mInfos{std::move(other.mInfos)}
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
                        mVbo            = std::move(other.mVbo);
                        mEbo            = std::move(other.mEbo);
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

