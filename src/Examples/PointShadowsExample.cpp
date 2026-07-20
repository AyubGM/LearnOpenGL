#include "PointShadowsExample.h"
#include <GLFW/glfw3.h>

PointShadowsExample::PointShadowsExample(uint32_t& width, uint32_t& height, Camera& camera) :
    m_Shader("src/shaders/PointShadows/3.2.2.point_shadows.vert", "src/shaders/PointShadows/3.2.2.point_shadows.frag"),
    m_SimpleDepthShader("src/shaders/PointShadows/3.2.2.point_shadows_depth.vert", "src/shaders/PointShadows/3.2.2.point_shadows_depth.frag", "src/shaders/PointShadows/3.2.2.point_shadows_depth.geom"),
    m_DepthMapFBO([&] {
    FramebufferSpecification spec;
    spec.Width = 1024;
    spec.Height = 1024;
    spec.Attachments = FramebufferAttachmentSpecification({
    { FramebufferTextureFormat::Depth, TextureTarget::TextureCubeMap }
        });
    return spec;
        }()),
    m_WoodTexture(("src/Resources/textures/wood.png")),
    m_Width(width),
    m_Height(height),
    m_LightPos(-2.0f, 4.0f, -1.0f),
    m_Camera(camera)
{
    SetUpTextures();
    SetUpBuffers();
    SetUpShaders();
}

PointShadowsExample::~PointShadowsExample()
{
}

void PointShadowsExample::SetUpTextures()
{
}

void PointShadowsExample::SetUpBuffers()
{
    float planeVertices[] = {
        // positions            // normals         // texcoords
         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
         25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
    };

    m_PlaneVBO.SetData(planeVertices, sizeof(planeVertices));
    m_PlaneVBO.SetLayout({
        {ShaderDataType::Float3, "aPos"},
        {ShaderDataType::Float3, "aNormal"},
        {ShaderDataType::Float2, "aTexCoords"}
        });

    m_PlaneVAO.AddVertexBuffer(m_PlaneVBO);


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

void PointShadowsExample::SetUpShaders()
{
    m_Shader.setInt("diffuseTexture", 0);
    m_Shader.setInt("depthMap", 1);

}

void PointShadowsExample::Run(const glm::mat4& view, const glm::mat4& projection)
{
    
    m_LightPos.z = static_cast<float>(sin(glfwGetTime() * 0.5) * 3.0);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float near_plane = 1.0f, far_plane = 25.0f;

    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)m_DepthMapFBO.GetSpecification().Width / (float)m_DepthMapFBO.GetSpecification().Height, 
        near_plane, far_plane);
    std::vector<glm::mat4> shadowTransforms;
    shadowTransforms.push_back(shadowProj * glm::lookAt(m_LightPos, m_LightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(m_LightPos, m_LightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(m_LightPos, m_LightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(m_LightPos, m_LightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(m_LightPos, m_LightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(m_LightPos, m_LightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

    glViewport(0, 0, m_DepthMapFBO.GetSpecification().Width, m_DepthMapFBO.GetSpecification().Height);
    m_DepthMapFBO.Bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    m_SimpleDepthShader.Bind();
    for (unsigned int i = 0; i < 6; ++i)
        m_SimpleDepthShader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
    m_SimpleDepthShader.setFloat("far_plane", far_plane);
    m_SimpleDepthShader.setVec3("lightPos", m_LightPos);
    RenderScene(m_SimpleDepthShader);
    m_DepthMapFBO.UnBind();

    glViewport(0, 0, m_Width, m_Height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_Shader.Bind();
    m_Shader.setMat4("projection", projection);
    m_Shader.setMat4("view", view);
    m_Shader.setVec3("lightPos", m_LightPos);
    m_Shader.setVec3("viewPos", m_Camera.Position);
    m_Shader.setInt("shadows", true); // enable/disable shadows by pressing 'SPACE'
    m_Shader.setFloat("far_plane", far_plane);
    m_WoodTexture.Bind();
    glBindTextureUnit(1, m_DepthMapFBO.GetDepthAttachment());
    RenderScene(m_Shader);
}

void PointShadowsExample::ShadowsRun(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos)
{
    glm::vec3 m_LightPos(-2.0f, 4.0f, -1.0f);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float near_plane = 1.0f, far_plane = 7.5f;
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = glm::lookAt(m_LightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;
    m_SimpleDepthShader.Bind();
    m_SimpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

    glViewport(0, 0, m_DepthMapFBO.GetSpecification().Width, m_DepthMapFBO.GetSpecification().Height);
    m_DepthMapFBO.Bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    m_WoodTexture.Bind();
    RenderScene(m_SimpleDepthShader);
    m_DepthMapFBO.UnBind();

    glViewport(0, 0, m_Width, m_Height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_Shader.Bind();
    m_Shader.setMat4("projection", projection);
    m_Shader.setMat4("view", view);
    // set light uniforms
    m_Shader.setVec3("viewPos", cameraPos);
    m_Shader.setVec3("m_LightPos", m_LightPos);
    m_Shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    m_WoodTexture.Bind();
    glBindTextureUnit(1, m_DepthMapFBO.GetDepthAttachment());
    RenderScene(m_Shader);

}

void PointShadowsExample::RenderScene(const Shader& shader)
{
    // room cube
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(5.0f));
    shader.setMat4("model", model);
    glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
    shader.setInt("reverse_normals", 1); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
    RenderCube();
    shader.setInt("reverse_normals", 0); // and of course disable it
    glEnable(GL_CULL_FACE);

    // cubes
    // cubes
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(4.0f, -3.5f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    RenderCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 3.0f, 1.0));
    model = glm::scale(model, glm::vec3(0.75f));
    shader.setMat4("model", model);
    RenderCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    RenderCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    RenderCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.75f));
    shader.setMat4("model", model);
    RenderCube();
}

void PointShadowsExample::RenderQuad()
{
    m_QuadVAO.Bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void PointShadowsExample::RenderCube()
{
    m_CubeVAO.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}