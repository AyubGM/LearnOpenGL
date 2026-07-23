#include "NormalMappingExample.h"
#include <GLFW/glfw3.h>

NormalMappingExample::NormalMappingExample(uint32_t& width, uint32_t& height, Camera& camera) :
m_Shader("shaders/NormalMapping/4.normal_mapping.vert", "shaders/NormalMapping/4.normal_mapping.frag"),
m_DiffuseMap("Resources/textures/brickwall.jpg"),
m_NormalMap("Resources/textures/brickwall_normal.jpg"),
m_LightPos(0.5f, 1.0f, 0.3f),
m_Camera(camera)
{
	SetUpTextures();
	SetUpBuffers();
	SetUpShaders();
}

NormalMappingExample::~NormalMappingExample()
{
}

void NormalMappingExample::Run(const glm::mat4& view, const glm::mat4& projection)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_Shader.Bind();
	m_Shader.setMat4("projection", projection);
	m_Shader.setMat4("view", view);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians((float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0))); // rotate the quad to show normal mapping from multiple directions
	m_Shader.setMat4("model", model);
	m_Shader.setVec3("viewPos", m_Camera.Position);
	m_Shader.setVec3("lightPos", m_LightPos);
	m_DiffuseMap.Bind();
	m_NormalMap.Bind(1);
	RenderQuad();

	// render light source (simply re-renders a smaller plane at the light's position for debugging/visualization)
	model = glm::mat4(1.0f);
	model = glm::translate(model, m_LightPos);
	model = glm::scale(model, glm::vec3(0.1f));
	m_Shader.setMat4("model", model);
	RenderQuad();

}

void NormalMappingExample::RenderQuad()
{
	m_QuadVAO.Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void NormalMappingExample::SetUpTextures()
{
}

void NormalMappingExample::SetUpBuffers()
{
    // positions
    glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
    glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
    glm::vec3 pos3(1.0f, -1.0f, 0.0f);
    glm::vec3 pos4(1.0f, 1.0f, 0.0f);
    // texture coordinates
    glm::vec2 uv1(0.0f, 1.0f);
    glm::vec2 uv2(0.0f, 0.0f);
    glm::vec2 uv3(1.0f, 0.0f);
    glm::vec2 uv4(1.0f, 1.0f);
    // normal vector
    glm::vec3 nm(0.0f, 0.0f, 1.0f);

    // calculate tangent/bitangent vectors of both triangles
    glm::vec3 tangent1, bitangent1;
    glm::vec3 tangent2, bitangent2;
    // triangle 1
    // ----------
    glm::vec3 edge1 = pos2 - pos1;
    glm::vec3 edge2 = pos3 - pos1;
    glm::vec2 deltaUV1 = uv2 - uv1;
    glm::vec2 deltaUV2 = uv3 - uv1;

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

    bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

    // triangle 2
    // ----------
    edge1 = pos3 - pos1;
    edge2 = pos4 - pos1;
    deltaUV1 = uv3 - uv1;
    deltaUV2 = uv4 - uv1;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


    bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);


    float quadVertices[] = {
        // positions            // normal         // texcoords  // tangent                          // bitangent
        pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
        pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
        pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

        pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
        pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
        pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
    };

	m_QuadVBO.SetData(quadVertices, sizeof(quadVertices));
	m_QuadVBO.SetLayout({
		{ShaderDataType::Float3, "aPos"},
		{ShaderDataType::Float3, "aNormal"},
		{ShaderDataType::Float2, "aTexCoords"},
		{ShaderDataType::Float3, "aTangent"},
		{ShaderDataType::Float3, "aBitangent"},
		});

	m_QuadVAO.AddVertexBuffer(m_QuadVBO);
}

void NormalMappingExample::SetUpShaders()
{
    m_Shader.setInt("diffuseMap", 0);
    m_Shader.setInt("normalMap", 1);
}
