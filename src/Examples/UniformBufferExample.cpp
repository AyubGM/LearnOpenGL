#include "UniformBufferExample.h"



UniformBufferExample::UniformBufferExample() : 
	m_ShaderRed("src/shaders/UnifomBuffer/8.advanced_glsl.vert", "src/shaders/UnifomBuffer/8.red.frag"),
	m_ShaderGreen("src/shaders/UnifomBuffer/8.advanced_glsl.vert", "src/shaders/UnifomBuffer/8.green.frag"),
	m_ShaderBlue("src/shaders/UnifomBuffer/8.advanced_glsl.vert", "src/shaders/UnifomBuffer/8.blue.frag"),
	m_ShaderYellow("src/shaders/UnifomBuffer/8.advanced_glsl.vert", "src/shaders/UnifomBuffer/8.yellow.frag"),
	m_UboMatrices(2 * sizeof(glm::mat4), 0)
{
    SetUpTextures();
    SetUpBuffers();
    SetUpShaders();
}

UniformBufferExample::~UniformBufferExample()
{
}

void UniformBufferExample::Run(const glm::mat4& view, const glm::mat4& projection)
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_UboMatrices.SetData(0, sizeof(projection), &projection);
	m_UboMatrices.SetData(sizeof(projection), sizeof(view), &view);

    m_CubeVAO.Bind();
    m_ShaderRed.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-0.75f, 0.75f, 0.0f));
    m_ShaderRed.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    m_ShaderGreen.use();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.75f, 0.75f, 0.0f)); // move top-right
    m_ShaderGreen.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    m_ShaderYellow.use();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-0.75f, -0.75f, 0.0f)); // move bottom-left
    m_ShaderYellow.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    m_ShaderBlue.use();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.75f, -0.75f, 0.0f)); // move bottom-right
    m_ShaderBlue.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    m_ShaderYellow.use();
}

void UniformBufferExample::SetUpTextures()
{
}

void UniformBufferExample::SetUpBuffers()
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
        -0.5f,  0.5f, -0.5f,
    };
    m_CubeVB.SetData(cubeVertices, sizeof(cubeVertices));
    m_CubeVB.SetLayout({{ ShaderDataType::Float3, "aPos" }});

    m_CubeVAO.AddVertexBuffer(m_CubeVB);
}

void UniformBufferExample::SetUpShaders()
{
}
