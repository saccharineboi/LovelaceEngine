#include "glmodel.h"
#include "error.h"

namespace al::gl
{
        ////////////////////////////////////////////////////////////////////////////////
        model::model(const std::string& path, texture_loader& textureLoader)
                : mPath{path}
        {
                Assimp::Importer importer;
                const aiScene* ai_scene = importer.ReadFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
                if (!ai_scene || ai_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !ai_scene->mRootNode)
                        throw exception("al::gl", "model", "model", importer.GetErrorString(), etype::unexpected);

                processNode(ai_scene->mRootNode, ai_scene, textureLoader);
        }

        ////////////////////////////////////////////////////////////////////////////////
        void model::processNode(aiNode* ai_node, const aiScene* ai_scene, texture_loader& textureLoader)
        {
                for (size_t i = 0; i < ai_node->mNumMeshes; ++i) {
                        aiMesh* ai_mesh = ai_scene->mMeshes[ai_node->mMeshes[i]];
                        mMeshes.push_back(processMesh(ai_mesh, ai_scene, textureLoader));
                }

                for (size_t i = 0; i < ai_node->mNumChildren; ++i)
                        processNode(ai_node->mChildren[i], ai_scene, textureLoader);
        }

        ////////////////////////////////////////////////////////////////////////////////
        static std::string genTexturePath(std::string modelPath, const char* textureUrl)
        {
                int pos = (int)modelPath.find_last_of('/');
                if (pos >= 0) {
                        modelPath.erase(pos + 1);
                        modelPath.append(textureUrl);
                        return modelPath;
                }
                throw exception("al::gl", "", "genTexturePath", modelPath + " is an invalid path", etype::unexpected);
        }

        ////////////////////////////////////////////////////////////////////////////////
        mesh model::processMesh(aiMesh* ai_mesh, const aiScene* ai_scene, texture_loader& textureLoader)
        {
                // process vertices
                std::vector<float> vertices;
                for (size_t i = 0; i < ai_mesh->mNumVertices; ++i) {
                        vertices.push_back(ai_mesh->mVertices[i].x);
                        vertices.push_back(ai_mesh->mVertices[i].y);
                        vertices.push_back(ai_mesh->mVertices[i].z);

                        vertices.push_back(ai_mesh->mNormals[i].x);
                        vertices.push_back(ai_mesh->mNormals[i].y);
                        vertices.push_back(ai_mesh->mNormals[i].z);

                        if (ai_mesh->mTextureCoords[0]) {
                                vertices.push_back(ai_mesh->mTextureCoords[0][i].x);
                                vertices.push_back(ai_mesh->mTextureCoords[0][i].y);
                        }
                        else {
                                vertices.push_back(0.0f);
                                vertices.push_back(0.0f);
                        }
                }

                // process indices
                std::vector<unsigned> indices;
                for (size_t i = 0; i < ai_mesh->mNumFaces; ++i) {
                        aiFace ai_face = ai_mesh->mFaces[i];
                        for (size_t j = 0; j < ai_face.mNumIndices; ++j)
                                indices.push_back(ai_face.mIndices[j]);
                }

                // process material
                std::vector<texture2D*> textures;
                // if mMaterialIndex is unsigned, then why check >= 0 ?
                if ((int)ai_mesh->mMaterialIndex >= 0) {
                        aiMaterial* ai_material = ai_scene->mMaterials[ai_mesh->mMaterialIndex];
                        if (ai_material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
                                aiString str;
                                ai_material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
                                std::string texturePath = genTexturePath(mPath, str.C_Str());
                                textures.push_back(textureLoader.load2D(texturePath));
                        }
                        if (ai_material->GetTextureCount(aiTextureType_SPECULAR) > 0) {
                                aiString str;
                                ai_material->GetTexture(aiTextureType_SPECULAR, 0, &str);
                                std::string texturePath = genTexturePath(mPath, str.C_Str());
                                textures.push_back(textureLoader.load2D(texturePath));
                        }
                }

                // create mesh
                std::vector<vao_info> infos {
                        { 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0 },
                        { 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)) },
                        { 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)) }
                };
                mesh m(std::move(vertices), std::move(indices), infos);
                m.mTextures = textures;
                return m;
        }

        ////////////////////////////////////////////////////////////////////////////////
        void model::draw(int mode) const
        {
                for (const mesh& m : mMeshes)
                        m.draw(mode);
        }
}
