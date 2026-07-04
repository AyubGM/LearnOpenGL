#include "CubeMapExample.h"

#include "../TextureLoader.h"





CubeMapExample::CubeMapExample() :
    m_Shader("src/shaders/CubeMap/6.1.cubemaps.vert", "src/shaders/CubeMap/6.1.cubemaps.frag"),
    m_SkyboxShader("src/shaders/CubeMap/6.1.skybox.vert", "src/shaders/CubeMap/6.1.skybox.frag")
{
    SetUpTextures();
    SetUpBuffers();
    SetUpShaders();
}

CubeMapExample::~CubeMapExample()
{
}

void CubeMapExample::SetUpTextures()
{
    std::vector<std::string> faces = {
        "src/Textures/skybox/right.jpg",
        "src/Textures/skybox/left.jpg",
        "src/Textures/skybox/top.jpg",
        "src/Textures/skybox/bottom.jpg",
        "src/Textures/skybox/front.jpg",
        "src/Textures/skybox/back.jpg"
    };
    m_CubeTexture = TextureLoader::LoadTexture("src/textures/container.jpg");
    m_SkyboxTexture = TextureLoader::LoadCubemap(faces);
    
}

void CubeMapExample::SetUpBuffers()
{
    float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    m_CubeVB.SetData(cubeVertices, sizeof(cubeVertices));
    m_CubeVB.SetLayout({
    { ShaderDataType::Float3, "aPos" },
    { ShaderDataType::Float2, "aTexCoords" }
        });

    m_CubeVAO.AddVertexBuffer(m_CubeVB);

    m_SkyboxVB.SetData(skyboxVertices, sizeof(skyboxVertices));
    m_SkyboxVB.SetLayout({ {ShaderDataType::Float3, "aPos"} });

    m_SkyboxVAO.AddVertexBuffer(m_SkyboxVB);

   
}

void CubeMapExample::SetUpShaders()
{
    m_Shader.use();
    m_Shader.setInt("texture1", 0);

    m_SkyboxShader.use();
    m_SkyboxShader.setInt("skybox", 0);
}

void CubeMapExample::Run(const glm::mat4& view, const glm::mat4& projection)
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_Shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    m_Shader.setMat4("model", model);
    m_Shader.setMat4("view", view);
    m_Shader.setMat4("projection", projection);

    // cubes
    m_CubeVAO.Bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_CubeTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // draw skybox as last
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    m_SkyboxShader.use();
    glm::mat4 noTranslationViewMat = glm::mat4(glm::mat3(view));    // remove translation from the view matrix
    m_SkyboxShader.setMat4("view", noTranslationViewMat);
    m_SkyboxShader.setMat4("projection", projection);
    m_SkyboxVAO.Bind();
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_SkyboxTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);

}

void CubeMapExample::Exercise(glm::mat4& view, glm::mat4& projection, Camera camera)
{
    
}
