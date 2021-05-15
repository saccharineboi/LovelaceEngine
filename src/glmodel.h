#pragma once

#include <vector>
#include <map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "glmesh.h"
#include "glmaterial.h"

namespace al::gl
{
        ////////////////////////////////////////////////////////////////////////////////
        class model
        {
                std::string mPath;
                std::vector<mesh> mMeshes;

                void processNode(aiNode* ai_node, const aiScene* ai_scene);
                mesh processMesh(aiMesh* ai_mesh, const aiScene* ai_scene);
        public:
                explicit model(const std::string& path);

                void draw(abstract_material& material, int mode = GL_TRIANGLES) const;

                std::string getPath() const                             { return mPath; }
                size_t getNumMeshes() const                             { return mMeshes.size(); }
        };
}
