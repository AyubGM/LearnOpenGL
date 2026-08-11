#include "HDRExample.h"
#include "../Events/EventDispatcher.h"
#include <GLFW/glfw3.h>
#include <iostream>

HDRExample::HDRExample(uint32_t width, uint32_t height, Camera& camera) : m_Shader("Shaders/HDR/6.lighting.vert", "Shaders/HDR/6.lighting.frag"),
m_HdrShader("Shaders/HDR/6.hdr.vert", "Shaders/HDR/6.hdr.frag"),
m_WoodTexture("Resources/textures/wood.png", "texture_diffuse", true),
m_HdrFrameBuffer([&] {
    FramebufferSpecification spec;
    spec.Width = width;
    spec.Height = height;
    spec.Attachments = { FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::Depth };
    return FrameBuffer(spec);
    }()),
	m_Camera(camera)
{
	SetUpTextures();
	SetUpBuffers();
	SetUpShaders();

    m_LightPositions.push_back(glm::vec3(0.0f, 0.0f, 49.5f)); // back light
    m_LightPositions.push_back(glm::vec3(-1.4f, -1.9f, 9.0f));
    m_LightPositions.push_back(glm::vec3(0.0f, -1.8f, 4.0f));
    m_LightPositions.push_back(glm::vec3(0.8f, -1.7f, 6.0f));

    m_LightColors.push_back(glm::vec3(200.0f, 200.0f, 200.0f));
    m_LightColors.push_back(glm::vec3(0.1f, 0.0f, 0.0f));
    m_LightColors.push_back(glm::vec3(0.0f, 0.0f, 0.2f));
    m_LightColors.push_back(glm::vec3(0.0f, 0.1f, 0.0f));

    g_EventDispatcher.AddEventListener<KeyEvent>([this](const KeyEvent& e) {
        bool keyPressed = (e.Action == GLFW_PRESS);
        bool prevHdrState = m_Hdr;

        switch (e.Key) {
        case GLFW_KEY_H:     m_Hdr = keyPressed? !m_Hdr : m_Hdr; break;
		case GLFW_KEY_Q:         m_Exposure -= 0.1f; break;
		case GLFW_KEY_E:         m_Exposure += 0.1f; break;

        }

		if (prevHdrState != m_Hdr) std::cout << "HDR Enabled: " << m_Hdr << std::endl;
        std::cout << "Exposure: " << m_Exposure << std::endl;
        });
}

void HDRExample::SetUpTextures()
{
    
}

void HDRExample::SetUpBuffers()
{
    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    m_QuadVBO.SetData(quadVertices, sizeof(quadVertices));
    m_QuadVBO.SetLayout({
        {ShaderDataType::Float3, "aPos"},
        {ShaderDataType::Float2, "aTexCoords"}
        });

    m_QuadVAO.AddVertexBuffer(m_QuadVBO);


    float cubeVertices[] = {
        // back face
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
         1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
        // front face
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
         1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
        // left face
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        // right face
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
         // bottom face
         -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
          1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
          1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
          1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
         -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
         -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
         // top face
         -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
          1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
          1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
          1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
         -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
         -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
    };

    m_CubeVBO.SetData(cubeVertices, sizeof(cubeVertices));
    m_CubeVBO.SetLayout({
        {ShaderDataType::Float3, "aPos"},
        {ShaderDataType::Float3, "aNormal"},
        {ShaderDataType::Float2, "aTexCoords"}
        });

    m_CubeVAO.AddVertexBuffer(m_CubeVBO);
}

void HDRExample::SetUpShaders()
{
	m_Shader.setInt("diffuseTexture", 0);
	m_HdrShader.setInt("hdrTexture", 0);
}

void HDRExample::Run(const glm::mat4& view, const glm::mat4& projection)
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_HdrFrameBuffer.Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_Shader.setMat4("projection", projection);
    m_Shader.setMat4("view", view);
    for (unsigned int i = 0; i < m_LightPositions.size(); i++)
    {
        m_Shader.setVec3("lights[" + std::to_string(i) + "].Position", m_LightPositions[i]);
        m_Shader.setVec3("lights[" + std::to_string(i) + "].Color", m_LightColors[i]);
    }
    m_Shader.setVec3("viewPos", m_Camera.Position);
    // render tunnel
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0));
    model = glm::scale(model, glm::vec3(2.5f, 2.5f, 27.5f));
    m_Shader.setMat4("model", model);
    m_Shader.setInt("inverse_normals", true);
    m_Shader.Bind();
    m_WoodTexture.Bind();
    RenderCube();
	m_HdrFrameBuffer.UnBind();

    // 2. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
        // --------------------------------------------------------------------------------------------------------------------------
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_HdrShader.Bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_HdrFrameBuffer.GetColorAttachment());
    m_HdrShader.setInt("hdr", m_Hdr);
    m_HdrShader.setFloat("exposure", m_Exposure);
    RenderQuad();

}

void HDRExample::RenderQuad()
{
    m_QuadVAO.Bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void HDRExample::RenderCube()
{
    m_CubeVAO.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
