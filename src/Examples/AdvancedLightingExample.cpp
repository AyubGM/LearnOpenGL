#include "AdvancedLightingExample.h"

AdvancedLightingExample::AdvancedLightingExample(Camera& camera) : m_Shader("src/shaders/AdvancedLighting/1.advanced_lighting.vert","src/shaders/AdvancedLighting/1.advanced_lighting.frag"),
m_FloorTexture("src/Resources/textures/wood.png"),
m_Camera(camera)
{
	SetUpTextures();
	SetUpBuffers();
	SetUpShaders();
}

AdvancedLightingExample::~AdvancedLightingExample()
{
}

void AdvancedLightingExample::Run(const glm::mat4& view, const glm::mat4& projection)
{
    glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_Shader.Bind();
    m_Shader.setMat4("projection", projection);
    m_Shader.setMat4("view", view);
    // set light uniforms
    m_Shader.setVec3("viewPos", m_Camera.Position);
    m_Shader.setVec3("lightPos", lightPos);
    m_Shader.setInt("blinn", m_Blinn);

    m_PlaneVAO.Bind();
    m_FloorTexture.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);

    std::cout << (m_Blinn ? "Blinn-Phong" : "Phong") << std::endl;

}


void AdvancedLightingExample::SetUpTextures()
{
}

void AdvancedLightingExample::SetUpBuffers()
{
    float planeVertices[] = {
        // positions            // normals         // texcoords
         10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
        -10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

         10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
        -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
         10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
    };
    m_PlaneVBO.SetData(planeVertices, sizeof(planeVertices));
    m_PlaneVBO.SetLayout({ {
        {ShaderDataType::Float3, "aPos"},
        {ShaderDataType::Float3, "aNormal"},
        {ShaderDataType::Float2, "aTexCoords"},
        } });
    m_PlaneVAO.AddVertexBuffer(m_PlaneVBO);
}

void AdvancedLightingExample::SetUpShaders()
{
}

