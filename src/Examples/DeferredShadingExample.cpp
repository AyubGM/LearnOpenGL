#include "DeferredShadingExample.h"

DeferredShadingExample::DeferredShadingExample(uint32_t& width, uint32_t& height, Camera& camera) : 
	m_ShaderGeometryPass("Shaders/DeferredShading/8.2.g_buffer.vert", "Shaders/DeferredShading/8.2.g_buffer.frag"),
	m_ShaderLightingPass("Shaders/DeferredShading/8.2.deferred_shading.vert", "Shaders/DeferredShading/8.2.deferred_shading.frag"),
	m_ShaderLightBox("Shaders/DeferredShading/8.2.deferred_light_box.vert", "Shaders/DeferredShading/8.2.deferred_light_box.frag"),
	//m_Backpack("Resources/Objects/backpack/backpack.obj"),
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
    m_ContainerTexture("resources/textures/container2.png"),
    m_ContainerTextureSpec("resources/textures/container2_specular.png")
{
    SetUpTextures();
    SetUpBuffers();
    SetUpShaders();

    m_ObjectPositions.push_back(glm::vec3(-3.0, -0.5, -3.0));
    m_ObjectPositions.push_back(glm::vec3(0.0, -0.5, -3.0));
    m_ObjectPositions.push_back(glm::vec3(3.0, -0.5, -3.0));
    m_ObjectPositions.push_back(glm::vec3(-3.0, -0.5, 0.0));
    m_ObjectPositions.push_back(glm::vec3(0.0, -0.5, 0.0));
    m_ObjectPositions.push_back(glm::vec3(3.0, -0.5, 0.0));
    m_ObjectPositions.push_back(glm::vec3(-3.0, -0.5, 3.0));
    m_ObjectPositions.push_back(glm::vec3(0.0, -0.5, 3.0));
    m_ObjectPositions.push_back(glm::vec3(3.0, -0.5, 3.0));

    std::srand(13);
    for (unsigned int i = 0; i < m_NRLIGHTS; i++)
    {
        // calculate slightly random offsets
        float xPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
        float yPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 4.0);
        float zPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
        m_LightPositions.push_back(glm::vec3(xPos, yPos, zPos));

        // also calculate random color
        float rColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.)
        float gColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.)
        float bColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.)
        m_LightColors.push_back(glm::vec3(rColor, gColor, bColor));
    }
}

DeferredShadingExample::~DeferredShadingExample()
{
}


void DeferredShadingExample::SetUpTextures()
{
}

void DeferredShadingExample::SetUpBuffers()
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

void DeferredShadingExample::SetUpShaders()
{
    m_ShaderLightingPass.setInt("gPosition", 0);
    m_ShaderLightingPass.setInt("gNormal", 1);
    m_ShaderLightingPass.setInt("gAlbedoSpec", 2);
}

void DeferredShadingExample::Run(const glm::mat4& view, const glm::mat4& projection)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 1. geometry pass: render scene's geometry/color data into gbuffer
    // -----------------------------------------------------------------
    m_GBuffer.Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 model = glm::mat4(1.0f);
    m_ShaderGeometryPass.Bind();
    m_ShaderGeometryPass.setMat4("projection", projection);
    m_ShaderGeometryPass.setMat4("view", view);
    for (unsigned int i = 0; i < m_ObjectPositions.size(); i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, m_ObjectPositions[i]);
        model = glm::scale(model, glm::vec3(0.25f));
        m_ShaderGeometryPass.setMat4("model", model);
        m_ContainerTexture.Bind();
        m_ContainerTextureSpec.Bind(1);
        RenderCube();
        //m_Backpack.Draw(m_ShaderGeometryPass);
    }
    m_GBuffer.UnBind();

    // 2. lighting pass: calculate lighting by iterating over a screen filled quad pixel-by-pixel using the gbuffer's content.
       // -----------------------------------------------------------------------------------------------------------------------
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_ShaderLightingPass.Bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_GBuffer.GetColorAttachment(0));
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_GBuffer.GetColorAttachment(1));
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_GBuffer.GetColorAttachment(2));

    // send light relevant uniforms
    for (uint32_t i = 0; i < m_LightPositions.size(); i++)
    {
        m_ShaderLightingPass.setVec3("lights[" + std::to_string(i) + "].Position", m_LightPositions[i]);
        m_ShaderLightingPass.setVec3("lights[" + std::to_string(i) + "].Color", m_LightColors[i]);
        // update attenuation parameters and calculate radius
        const float constant = 1.0f; // note that we don't send this to the shader, we assume it is always 1.0 (in our case)
        const float linear = 0.7f;
        const float quadratic = 1.8f;
        m_ShaderLightingPass.setFloat("lights[" + std::to_string(i) + "].Linear", linear);
        m_ShaderLightingPass.setFloat("lights[" + std::to_string(i) + "].Quadratic", quadratic);
        // then calculate radius of light volume/sphere
        const float maxBrightness = std::fmaxf(std::fmaxf(m_LightColors[i].r, m_LightColors[i].g), m_LightColors[i].b);
        float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);
        m_ShaderLightingPass.setFloat("lights[" + std::to_string(i) + "].Radius", radius);
    }
    m_ShaderLightingPass.setVec3("viewPos", m_Camera.Position);
    // finally render quad
    RenderQuad();

    FrameBuffer::ReadFromReadTo(m_GBuffer.GetID(), 0, m_Width, m_Height, true);
    

    // 3. render lights on top of scene
    // --------------------------------
    m_ShaderLightBox.Bind();
    m_ShaderLightBox.setMat4("projection", projection);
    m_ShaderLightBox.setMat4("view", view);
    for (unsigned int i = 0; i < m_LightPositions.size(); i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, m_LightPositions[i]);
        model = glm::scale(model, glm::vec3(0.125f));
        m_ShaderLightBox.setMat4("model", model);
        m_ShaderLightBox.setVec3("lightColor", m_LightColors[i]);
        RenderCube();
    }

}

void DeferredShadingExample::RenderQuad()
{
    m_QuadVAO.Bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void DeferredShadingExample::RenderCube()
{
    m_CubeVAO.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}