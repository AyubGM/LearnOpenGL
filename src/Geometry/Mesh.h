#pragma once

#include "../Buffers/VertexArray.h"
#include "../Buffers/VertexBuffer.h"
#include "../Buffers/IndexBuffer.h"
#include"../Shader.h"

#include <glad/glad.h> // holds all OpenGL type declarations
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>
#include "../Texture/Texture2D.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned int Id;
    std::string Type;
    std::string Path;
};

class Mesh {
public:
    Mesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices, std::vector<std::shared_ptr<Texture2D>>&& textures);

    void Draw(Shader& shader) const;
    void DrawInstanced(Shader& shader, uint32_t instanceCount) const;

    void AddInstanceBuffer(const VertexBuffer& instanceBuffer);


public:
    // mesh Data
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
    std::vector<std::shared_ptr<Texture2D>> m_Textures;
  
private:
    VertexArray m_VAO;
    VertexBuffer m_VBO;
    IndexBuffer m_EBO;

    void setupMesh();
};