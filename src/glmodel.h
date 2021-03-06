#pragma once

#include "glmesh.h"
#include "gltexture_loader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <map>

namespace al::gl
{
        ////////////////////////////////////////////////////////////////////////////////
        class model
        {
                std::string mPath;
                std::vector<mesh> mMeshes;

                void processNode(aiNode* ai_node, const aiScene* ai_scene, texture_loader& loader);
                mesh processMesh(aiMesh* ai_mesh, const aiScene* ai_scene, texture_loader& loader);
        public:
                model(const std::string& path, texture_loader& loader);

                void draw(int mode = GL_TRIANGLES) const;

                std::string getPath() const                             { return mPath; }
                size_t getNumMeshes() const                             { return mMeshes.size(); }
        };

        ////////////////////////////////////////////////////////////////////////////////
        inline bool operator==(const model& first, const model& second) { return first.getPath() == second.getPath(); }

        ////////////////////////////////////////////////////////////////////////////////
        inline bool operator!=(const model& first, const model& second) { return first.getPath() != second.getPath(); }
}
