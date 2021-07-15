#pragma once

#include "glbuffer.h"
#include "glvao.h"
#include "gltexture2D.h"

#include <vector>
#include <string>

namespace al::gl
{
        ////////////////////////////////////////////////////////////////////////////////
        class mesh
        {
                std::vector<vao_info> mInfos;
                vao<float, unsigned> mVao;

                void bindTextures() const
                {
                        for (int i = 0; i < static_cast<int>(mTextures.size()); ++i)
                                mTextures[i]->bind(i);
                }

                void unbindTextures() const
                {
                        for (int i = static_cast<int>(mTextures.size()) - 1; i >= 0; --i)
                                mTextures[i]->unbind(i);
                }
        public:
                std::vector<texture2D*> mTextures;

                mesh(std::vector<float>&& vertices, std::vector<unsigned>&& indices, const std::vector<vao_info>& infos);

                void draw(int mode = GL_TRIANGLES) const
                {
                        bindTextures();
                        mVao.bind();
                        mVao.draw(mode);
                        mVao.unbind();
                        unbindTextures();
                }
        };

        ////////////////////////////////////////////////////////////////////////////////
        mesh genTriangle();

        ////////////////////////////////////////////////////////////////////////////////
        mesh genColoredTriangle();

        ////////////////////////////////////////////////////////////////////////////////
        mesh genQuad();

        ////////////////////////////////////////////////////////////////////////////////
        mesh genCube();
}
