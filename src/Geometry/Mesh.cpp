#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<unsigned int>&& indices, std::vector<std::shared_ptr<Texture2D>>&& textures)
    : m_Vertices(std::move(vertices)), m_Indices(std::move(indices)), m_Textures(std::move(textures))
{
    setupMesh();
}

void Mesh::AddInstanceBuffer(const VertexBuffer& instanceBuffer)
{
    m_VAO.AddVertexBuffer(instanceBuffer);
}

// render the mesh
void Mesh::Draw(Shader& shader) const
{
    // bind appropriate textures
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for (unsigned int i = 0; i < m_Textures.size(); i++)
    {
        std::string number;
        std::string name = m_Textures[i]->GetType();

        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);
        else if (name == "texture_normal")
            number = std::to_string(normalNr++);
        else if (name == "texture_height")
            number = std::to_string(heightNr++);

        glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
        m_Textures[i]->Bind(i);
    }

    // draw mesh
    m_VAO.Bind();
    glDrawElements(GL_TRIANGLES, m_EBO.GetCount(), GL_UNSIGNED_INT, 0);
    m_VAO.UnBind();
}

void Mesh::DrawInstanced(Shader& shader, uint32_t instanceCount) const
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;

    for (unsigned int i = 0; i < m_Textures.size(); i++)
    {

        std::string number;
        std::string name = m_Textures[i]->GetType();

        if (name == "texture_diffuse") number = std::to_string(diffuseNr++);
        else if (name == "texture_specular") number = std::to_string(specularNr++);
        else if (name == "texture_normal") number = std::to_string(normalNr++);
        else if (name == "texture_height") number = std::to_string(heightNr++);

        glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
        m_Textures[i]->Bind(i);
    }

    m_VAO.Bind();
    // The magic function:
    glDrawElementsInstanced(GL_TRIANGLES, m_EBO.GetCount(), GL_UNSIGNED_INT, 0, instanceCount);
    m_VAO.UnBind();
}


void Mesh::setupMesh()
{
    // create buffers/arrays
    m_VAO = VertexArray();

    m_VBO = VertexBuffer(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));

    BufferLayout layout = {
        { ShaderDataType::Float3, "a_Position" },
        { ShaderDataType::Float3, "a_Normal" },
        { ShaderDataType::Float2, "a_TexCoords" },
        { ShaderDataType::Float3, "a_Tangent" },
        { ShaderDataType::Float3, "a_Bitangent" },
        { ShaderDataType::Int4,   "a_BoneIDs" },
        { ShaderDataType::Float4, "a_Weights" }
    };

    m_VBO.SetLayout(layout);
    m_VAO.AddVertexBuffer(m_VBO);

    m_EBO = IndexBuffer(m_Indices.data(), m_Indices.size());
    m_VAO.SetIndexBuffer(m_EBO);

}