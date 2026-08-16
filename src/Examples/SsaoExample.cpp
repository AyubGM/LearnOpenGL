#include "SsaoExample.h"

#include <random>

SsaoExample::SsaoExample(uint32_t& width, uint32_t& height, Camera& camera) :
    m_ShaderGeometryPass("Shaders/Ssao/9.ssao_geometry.vert", "Shaders/Ssao/9.ssao_geometry.frag"),
    m_ShaderLightingPass("Shaders/Ssao/9.ssao.vert", "Shaders/Ssao/9.ssao_lighting.frag"),
    m_ShaderSSAO("Shaders/Ssao/9.ssao.vert", "Shaders/Ssao/9.ssao.frag"),
    m_ShaderSSAOBlur("Shaders/Ssao/9.ssao.vert", "Shaders/Ssao/9.ssao_blur.frag"),
    m_Backpack("Resources/Objects/backpack/backpack.obj"),
    m_Width(width),
    m_Height(height),
    m_Camera(camera),
    m_GBuffer([&] {
    FramebufferSpecification spec;
    spec.Width = m_Width;
    spec.Height = m_Height;
    spec.Attachments = { FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };
    return FrameBuffer(spec);
        }()),
    m_SsaoFBO([&] {
    FramebufferSpecification spec;
    spec.Width = m_Width;
    spec.Height = m_Height;
    spec.Attachments = { FramebufferTextureFormat::RED};
    return FrameBuffer(spec);
        }()),
    m_SsaoBlurFBO([&] {
    FramebufferSpecification spec;
    spec.Width = m_Width;
    spec.Height = m_Height;
    spec.Attachments = { FramebufferTextureFormat::RED};
    return FrameBuffer(spec);
        }())
{
    SetUpTextures();
    SetUpBuffers();
    SetUpShaders();

    // generate sample kernel
   // ----------------------
    std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
    std::default_random_engine generator;
    for (unsigned int i = 0; i < 64; ++i)
    {
        glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
        sample = glm::normalize(sample);
        sample *= randomFloats(generator);
        float scale = float(i) / 64.0f;

        // scale samples s.t. they're more aligned to center of kernel
        scale = Lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        m_SsaoKernel.push_back(sample);
    }

    for (unsigned int i = 0; i < 16; i++)
    {
        glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
        m_SsaoNoise.push_back(noise);
    }


	m_NoiseTexture.CreateFromData(m_SsaoNoise.data(), 4, 4, GL_RGB32F, GL_RGB, GL_FLOAT, "ssao_noise");

    m_LightPosition = glm::vec3(2.0, 4.0, -2.0);
    m_LightColor = glm::vec3(0.2, 0.2, 0.7);

}

float SsaoExample::Lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

SsaoExample::~SsaoExample()
{
}


void SsaoExample::SetUpTextures()
{
}

void SsaoExample::SetUpBuffers()
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

void SsaoExample::SetUpShaders()
{
    m_ShaderLightingPass.setInt("gPosition", 0);
    m_ShaderLightingPass.setInt("gNormal", 1);
    m_ShaderLightingPass.setInt("gAlbedo", 2);
    m_ShaderLightingPass.setInt("ssao", 3);


    m_ShaderSSAO.setInt("gPosition", 0);
    m_ShaderSSAO.setInt("gNormal", 1);
    m_ShaderSSAO.setInt("texNoise", 2);

    m_ShaderSSAOBlur.setInt("ssaoInput", 0);
}

void SsaoExample::Run(const glm::mat4& view, const glm::mat4& projection)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 1. geometry pass: render scene's geometry/color data into gbuffer
    // -----------------------------------------------------------------
    m_GBuffer.Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_ShaderGeometryPass.Bind();
    m_ShaderGeometryPass.setMat4("projection", projection);
    m_ShaderGeometryPass.setMat4("view", view);
    
    // room cube
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0, 7.0f, 0.0f));
    model = glm::scale(model, glm::vec3(7.5f, 7.5f, 7.5f));
    m_ShaderGeometryPass.setMat4("model", model);
    m_ShaderGeometryPass.setInt("invertedNormals", 1); // invert normals as we're inside the cube
    RenderCube();

    // backpack model on the floor
    m_ShaderGeometryPass.setInt("invertedNormals", 0);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
    model = glm::scale(model, glm::vec3(1.0f));
    m_ShaderGeometryPass.setMat4("model", model);
    m_Backpack.Draw(m_ShaderGeometryPass);

    m_GBuffer.UnBind();


    // 2. SSAO pass
    // ------------------------
	m_SsaoFBO.Bind();
    glClear(GL_COLOR_BUFFER_BIT);
    m_ShaderSSAO.Bind();

    // Send kernel + rotation 
    for (unsigned int i = 0; i < 64; ++i)
        m_ShaderSSAO.setVec3("samples[" + std::to_string(i) + "]", m_SsaoKernel[i]);

    m_ShaderSSAO.setMat4("projection", projection);
    glActiveTexture(GL_TEXTURE0);
	
    glBindTexture(GL_TEXTURE_2D, m_GBuffer.GetColorAttachment(0));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_GBuffer.GetColorAttachment(1));
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_NoiseTexture.GetID());

    RenderQuad();
    m_SsaoFBO.UnBind();

    // 3. blur SSAO texture to remove noise
    // ------------------------------------
	m_SsaoBlurFBO.Bind();
	glClear(GL_COLOR_BUFFER_BIT);
    m_ShaderSSAOBlur.Bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_SsaoFBO.GetColorAttachment(0));
    RenderQuad();
    m_SsaoBlurFBO.UnBind();


    // 4. lighting pass: traditional deferred Blinn-Phong lighting with added screen-space ambient occlusion
    // -----------------------------------------------------------------------------------------------------
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_ShaderLightingPass.Bind();
    // send light relevant uniforms
    glm::vec3 lightPosView = glm::vec3(m_Camera.GetViewMatrix() * glm::vec4(m_LightPosition, 1.0));
    m_ShaderLightingPass.setVec3("light.Position", lightPosView);
    m_ShaderLightingPass.setVec3("light.Color", m_LightColor);
    // Update attenuation parameters
    const float linear = 0.09f;
    const float quadratic = 0.032f;
    m_ShaderLightingPass.setFloat("light.Linear", linear);
    m_ShaderLightingPass.setFloat("light.Quadratic", quadratic);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_GBuffer.GetColorAttachment(0));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_GBuffer.GetColorAttachment(1));
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_GBuffer.GetColorAttachment(2));
    glActiveTexture(GL_TEXTURE3); // add extra SSAO texture to lighting pass
    glBindTexture(GL_TEXTURE_2D, m_SsaoBlurFBO.GetColorAttachment(0));
    RenderQuad();

}

void SsaoExample::RenderQuad()
{
    m_QuadVAO.Bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void SsaoExample::RenderCube()
{
    m_CubeVAO.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}