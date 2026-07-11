#pragma once

#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include <iostream>
#include "../Texture/Texture2D.h"


class Model
{
public:
    std::vector<std::shared_ptr<Texture2D>> loadedTextures; // Global cache of loaded textures

    Model(std::string path);

    void Draw(Shader& shader);
    void DrawInstanced(Shader& shader, uint32_t instanceCount);


    void AddInstanceBuffer(const VertexBuffer& instanceBuffer);
    std::vector<Mesh>& GetMeshes() { return m_Meshes; }

private:
    std::string m_Directory;
    std::vector<Mesh> m_Meshes;


    void LoadModel(std::string path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

    
    std::vector<std::shared_ptr<Texture2D>> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

