#pragma once

#include "glbuffer.h"
#include "glvao.h"
#include "gltexture2D.h"
#include "error.h"

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
                        int count = static_cast<int>(mTextures.size());
                        switch (count) {
                                case 0:
                                        break;
                                case 1:
                                        mTextures[0]->bind(0);
                                        mTextures[0]->bind(1);
                                        mTextures[0]->bind(2);
                                        break;
                                case 2:
                                        mTextures[0]->bind(0);
                                        mTextures[0]->bind(1);
                                        mTextures[1]->bind(2);
                                        break;
                                case 3:
                                        mTextures[0]->bind(0);
                                        mTextures[1]->bind(1);
                                        mTextures[2]->bind(2);
                                        break;
                                default:
                                        throw exception("al::gl", "mesh", "bindTextures", "unexpected number of given textures for a mesh", etype::unexpected);
                        }
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
