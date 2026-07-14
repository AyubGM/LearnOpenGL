#include "AntiAliasingExample.h"


AntiAliasingExample::AntiAliasingExample(uint32_t& width, uint32_t& height) : m_Width(width), m_Height(height),
m_Shader("src/shaders/AntiAliasing/11.2.anti_aliasing.vert", "src/shaders/AntiAliasing/11.2.anti_aliasing.frag"),
m_ScreenShader("src/shaders/AntiAliasing/11.2.aa_post.vert", "src/shaders/AntiAliasing/11.2.aa_post.frag"),
m_MsaaFrameBuffer([&]{
    FramebufferSpecification msaaSpec;
    msaaSpec.Width = width;
    msaaSpec.Height = height;
    msaaSpec.Samples = 4;
    msaaSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::DEPTH24STENCIL8 };
    return FrameBuffer(msaaSpec);
    }()),
m_IntermediateFrameBuffer([&]{
    FramebufferSpecification intermediateSpec;
    intermediateSpec.Width = width;
    intermediateSpec.Height = height;
    //intermediateSpec.Samples = 1;
    intermediateSpec.Attachments = { FramebufferTextureFormat::RGBA8 };
    return FrameBuffer(intermediateSpec);
        }())
{
    

	SetUpTextures();
	SetUpBuffers();
	SetUpShaders();
}

AntiAliasingExample::~AntiAliasingExample()
{
}

void AntiAliasingExample::Run(const glm::mat4& view, const glm::mat4& projection)
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Hmmmmmmm I do not have event system
    m_MsaaFrameBuffer.Resize(m_Width, m_Height);
    m_MsaaFrameBuffer.Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    m_Shader.Bind();
    m_Shader.setMat4("projection", projection);
    m_Shader.setMat4("view", view);
    m_Shader.setMat4("model", glm::mat4(1.0f));

    m_CubeVAO.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // Hmmmmmmm I do not have event system
    m_MsaaFrameBuffer.Resize(m_Width, m_Height);
    FrameBuffer::ReadFromReadTo(m_MsaaFrameBuffer.GetID(), m_IntermediateFrameBuffer.GetID(), m_Width, m_Height);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    m_ScreenShader.Bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_MsaaFrameBuffer.GetColorAttachment());
    m_QuadVAO.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void AntiAliasingExample::SetUpTextures()
{
}

void AntiAliasingExample::SetUpBuffers()
{
    float cubeVertices[] = {
        // positions       
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    };
    float quadVertices[] = {   // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    m_CubeVBO.SetData(cubeVertices, sizeof(cubeVertices));
    m_CubeVBO.SetLayout({ {ShaderDataType::Float3, "aPos"} });

    m_CubeVAO.AddVertexBuffer(m_CubeVBO);

    m_QuadVBO.SetData(quadVertices, sizeof(quadVertices));
    m_QuadVBO.SetLayout({ 
        {ShaderDataType::Float2, "aPos"},
        {ShaderDataType::Float2, "aTexCoord"}
        });

    m_QuadVAO.AddVertexBuffer(m_QuadVBO);
}



void AntiAliasingExample::SetUpShaders()
{
    m_ScreenShader.setInt("screenTexture", 0);
}
