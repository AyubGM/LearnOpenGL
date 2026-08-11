#include "BloomExample.h"
#include <array>

BloomExample::BloomExample(uint32_t& width, uint32_t& height, Camera& camera) : m_Shader("Shaders/Bloom/7.bloom.vert", "Shaders/Bloom/7.bloom.frag"),
m_ShaderLight("Shaders/Bloom/7.bloom.vert", "Shaders/Bloom/7.light_box.frag"),
m_ShaderBlur("Shaders/Bloom/7.blur.vert", "Shaders/Bloom/7.blur.frag"),
m_ShaderBloomFinal("Shaders/Bloom/7.bloom_final.vert", "Shaders/Bloom/7.bloom_final.frag"),
m_WoodTexture("resources/textures/wood.png", "texture_diffuse", true),
m_ContainerTexture("resources/textures/container2.png", "texture_diffuse", true),
m_Width(width),
m_Height(height),
m_Camera(camera),
m_HdrFBO([&] {
    FramebufferSpecification spec;
    spec.Width = m_Width;
    spec.Height = m_Height;
    spec.Attachments = { FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::Depth };
    return FrameBuffer(spec);
    }()),
m_PingpongFBO1([&] {
    FramebufferSpecification spec;
    spec.Width = m_Width;
    spec.Height = m_Height;
    spec.Attachments = { FramebufferTextureFormat::RGBA16F};
    return FrameBuffer(spec);
        }()),
m_PingpongFBO2([&] {
    FramebufferSpecification spec;
    spec.Width = m_Width;
    spec.Height = m_Height;
    spec.Attachments = { FramebufferTextureFormat::RGBA16F };
    return FrameBuffer(spec);
        }())
{
    SetUpTextures();
    SetUpBuffers();
    SetUpShaders();

    m_LightPositions.push_back(glm::vec3(0.0f, 0.5f, 1.5f));
    m_LightPositions.push_back(glm::vec3(-4.0f, 0.5f, -3.0f));
    m_LightPositions.push_back(glm::vec3(3.0f, 0.5f, 1.0f));
    m_LightPositions.push_back(glm::vec3(-.8f, 2.4f, -1.0f));
    // colors
    m_LightColors.push_back(glm::vec3(5.0f, 5.0f, 5.0f));
    m_LightColors.push_back(glm::vec3(10.0f, 0.0f, 0.0f));
    m_LightColors.push_back(glm::vec3(0.0f, 0.0f, 15.0f));
    m_LightColors.push_back(glm::vec3(0.0f, 5.0f, 0.0f));
}

BloomExample::~BloomExample()
{
}

void BloomExample::SetUpTextures()
{
}

void BloomExample::SetUpBuffers()
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

void BloomExample::SetUpShaders()
{
    m_Shader.setInt("diffuseTexture", 0);

    m_ShaderBlur.setInt("image", 0);

    m_ShaderBloomFinal.setInt("scene", 0);
    m_ShaderBloomFinal.setInt("bloomBlur", 1);
}

void BloomExample::Run(const glm::mat4& view, const glm::mat4& projection)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_HdrFBO.Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model = glm::mat4(1.0f);
    m_Shader.Bind();
    m_Shader.setMat4("projection", projection);
    m_Shader.setMat4("view", view);
    m_WoodTexture.Bind();
    for (unsigned int i = 0; i < m_LightPositions.size(); i++)
    {
        m_Shader.setVec3("lights[" + std::to_string(i) + "].Position", m_LightPositions[i]);
        m_Shader.setVec3("lights[" + std::to_string(i) + "].Color", m_LightColors[i]);
    }
    m_Shader.setVec3("viewPos", m_Camera.Position);
    // create one large cube that acts as the floor
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0));
    model = glm::scale(model, glm::vec3(12.5f, 0.5f, 12.5f));
    m_Shader.setMat4("model", model);
    RenderCube();

    // then create multiple cubes as the scenery
    m_ContainerTexture.Bind();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    m_Shader.setMat4("model", model);
    RenderCube();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
    model = glm::scale(model, glm::vec3(0.5f));
    m_Shader.setMat4("model", model);
    RenderCube();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 2.0));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    m_Shader.setMat4("model", model);
    RenderCube();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 2.7f, 4.0));
    model = glm::rotate(model, glm::radians(23.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(1.25));
    m_Shader.setMat4("model", model);
    RenderCube();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -3.0));
    model = glm::rotate(model, glm::radians(124.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    m_Shader.setMat4("model", model);
    RenderCube();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    m_Shader.setMat4("model", model);
    RenderCube();

    // finally show all the light sources as bright cubes
    m_ShaderLight.Bind();
    m_ShaderLight.setMat4("projection", projection);
    m_ShaderLight.setMat4("view", view);
    for (unsigned int i = 0; i < m_LightPositions.size(); i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(m_LightPositions[i]));
        model = glm::scale(model, glm::vec3(0.25f));
        m_ShaderLight.setMat4("model", model);
        m_ShaderLight.setVec3("lightColor", m_LightColors[i]);
        RenderCube();
    }

    m_HdrFBO.UnBind();

    // 2. blur bright fragments with two-pass Gaussian Blur 
    // --------------------------------------------------
    bool horizontal = true, first_iteration = true;
    unsigned int amount = 10;
    m_ShaderBlur.Bind();
    std::array<FrameBuffer*, 2> pingpongFBOs = { &m_PingpongFBO1,  &m_PingpongFBO2};
    for (unsigned int i = 0; i < amount; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBOs[horizontal]->GetID());
        m_ShaderBlur.setInt("horizontal", horizontal);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, first_iteration ? m_HdrFBO.GetColorAttachment(1) : pingpongFBOs[!horizontal]->GetColorAttachment());  // bind texture of other framebuffer (or scene if first iteration)
        RenderQuad();
        horizontal = !horizontal;
        if (first_iteration)
            first_iteration = false;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 3. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
        // --------------------------------------------------------------------------------------------------------------------------
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_ShaderBloomFinal.Bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_HdrFBO.GetColorAttachment());
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, pingpongFBOs[!horizontal]->GetColorAttachment());
    m_ShaderBloomFinal.setInt("bloom", m_Bloom);
    m_ShaderBloomFinal.setFloat("exposure", m_Exposure);
    RenderQuad();
}

void BloomExample::RenderQuad()
{
    m_QuadVAO.Bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void BloomExample::RenderCube()
{
    m_CubeVAO.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}