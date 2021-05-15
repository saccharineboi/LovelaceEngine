#include "glmesh.h"

namespace al::gl
{
        ////////////////////////////////////////////////////////////////////////////////
        mesh::mesh(std::vector<float>&& vertices, std::vector<unsigned>&& indices, const std::vector<vao_info>& infos)
                : mVbo(GL_ARRAY_BUFFER, std::move(vertices), GL_STATIC_DRAW),
                  mEbo(GL_ELEMENT_ARRAY_BUFFER, std::move(indices), GL_STATIC_DRAW),
                  mInfos{(infos)},
                  mVao(mVbo, mEbo, mInfos) {}

        ////////////////////////////////////////////////////////////////////////////////
        mesh genTriangle()
        {
                std::vector<float> vertices {
                        // positions            // normals              // tex coords
                        -1.0f, -1.0f, 0.0f,     0.0f, 0.0f, 1.0f,       0.0f, 0.0f,
                         1.0f, -1.0f, 0.0f,     0.0f, 0.0f, 1.0f,       1.0f, 0.0f,
                         0.5f,  1.0f, 0.0f,     0.0f, 0.0f, 1.0f,       0.5f, 1.0f
                };

                std::vector<unsigned> indices {
                        0, 1, 2
                };

                std::vector<vao_info> infos {
                        { 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0 },
                        { 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)) },
                        { 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)) }
                };

                return mesh(std::move(vertices), std::move(indices), infos);
        }

        ////////////////////////////////////////////////////////////////////////////////
        mesh genColoredTriangle()
        {
                std::vector<float> vertices {
                        // positions            // colors
                        -1.0f, -1.0f, 0.0f,     1.0f, 0.0f, 0.0f,
                         1.0f, -1.0f, 0.0f,     0.0f, 1.0f, 0.0f,
                         0.5f,  1.0f, 0.0f,     0.0f, 0.0f, 1.0f
                };

                std::vector<unsigned> indices {
                        0, 1, 2
                };

                std::vector<vao_info> infos {
                        { 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0 },
                        { 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)) }
                };

                return mesh(std::move(vertices), std::move(indices), infos);
        }

        ////////////////////////////////////////////////////////////////////////////////
        mesh genQuad()
        {
                std::vector<float> vertices {
                        // positions            // normals              // tex coords
                        -1.0f, -1.0f, 0.0f,     0.0f, 0.0f, 1.0f,       0.0f, 0.0f,
                         1.0f, -1.0f, 0.0f,     0.0f, 0.0f, 1.0f,       1.0f, 0.0f,
                        -1.0f,  1.0f, 0.0f,     0.0f, 0.0f, 1.0f,       0.0f, 1.0f,
                         1.0f,  1.0f, 0.0f,     0.0f, 0.0f, 1.0f,       1.0f, 1.0f
                };

                std::vector<unsigned> indices {
                        0, 1, 2, 2, 1, 3
                };

                std::vector<vao_info> infos {
                        { 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0 },
                        { 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)) },
                        { 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)) }
                };

                return mesh(std::move(vertices), std::move(indices), infos);
        }

        ////////////////////////////////////////////////////////////////////////////////
        mesh genCube()
        {
                std::vector<float> vertices {
                        // positions           // nromals            // tex coords
                        // Front face
                        -1.0, -1.0,  1.0,      0.0,  0.0,  1.0,      0.0,  0.0,
                         1.0, -1.0,  1.0,      0.0,  0.0,  1.0,      1.0,  0.0,
                         1.0,  1.0,  1.0,      0.0,  0.0,  1.0,      1.0,  1.0,
                        -1.0,  1.0,  1.0,      0.0,  0.0,  1.0,      0.0,  1.0,
                        // Back face
                        -1.0, -1.0, -1.0,      0.0,  0.0, -1.0,      0.0,  0.0,
                        -1.0,  1.0, -1.0,      0.0,  0.0, -1.0,      1.0,  0.0,
                         1.0,  1.0, -1.0,      0.0,  0.0, -1.0,      1.0,  1.0,
                         1.0, -1.0, -1.0,      0.0,  0.0, -1.0,      0.0,  1.0,
                        // Top face
                        -1.0,  1.0, -1.0,      0.0,  1.0,  0.0,      0.0,  0.0,
                        -1.0,  1.0,  1.0,      0.0,  1.0,  0.0,      1.0,  0.0,
                         1.0,  1.0,  1.0,      0.0,  1.0,  0.0,      1.0,  1.0,
                         1.0,  1.0, -1.0,      0.0,  1.0,  0.0,      0.0,  1.0,
                        // Bottom face
                        -1.0, -1.0, -1.0,      0.0, -1.0,  0.0,      0.0,  0.0,
                         1.0, -1.0, -1.0,      0.0, -1.0,  0.0,      1.0,  0.0,
                         1.0, -1.0,  1.0,      0.0, -1.0,  0.0,      1.0,  1.0,
                        -1.0, -1.0,  1.0,      0.0, -1.0,  0.0,      0.0,  1.0,
                        // Right face
                         1.0, -1.0, -1.0,      1.0,  0.0,  0.0,      0.0,  0.0,
                         1.0,  1.0, -1.0,      1.0,  0.0,  0.0,      1.0,  0.0,
                         1.0,  1.0,  1.0,      1.0,  0.0,  0.0,      1.0,  1.0,
                         1.0, -1.0,  1.0,      1.0,  0.0,  0.0,      0.0,  1.0,
                        // Left face
                        -1.0, -1.0, -1.0,     -1.0,  0.0,  0.0,      0.0,  0.0,
                        -1.0, -1.0,  1.0,     -1.0,  0.0,  0.0,      1.0,  0.0,
                        -1.0,  1.0,  1.0,     -1.0,  0.0,  0.0,      1.0,  1.0,
                        -1.0,  1.0, -1.0,     -1.0,  0.0,  0.0,      0.0,  1.0
                };

                std::vector<unsigned> indices {
                        0,  1,  2,      0,  2,  3,    // front
                        4,  5,  6,      4,  6,  7,    // back
                        8,  9,  10,     8,  10, 11,   // top
                        12, 13, 14,     12, 14, 15,   // bottom
                        16, 17, 18,     16, 18, 19,   // right
                        20, 21, 22,     20, 22, 23    // left
                };

                std::vector<vao_info> infos {
                        { 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0 },
                        { 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)) },
                        { 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)) }
                };

                return mesh(std::move(vertices), std::move(indices), infos);
        }
}
