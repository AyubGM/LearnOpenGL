#include "InstancingExample.h"
#include <GLFW/glfw3.h>


InstancingExample::InstancingExample(uint32_t amount) :
m_AsteroidShader("src/shaders/Instancing/10.3.asteroids.vert", "src/shaders/Instancing/10.3.asteroids.frag"),
m_PlanetShader("src/shaders/Instancing/10.3.planet.vert", "src/shaders/Instancing/10.3.planet.frag"), 
m_RockModel("src/Resources/rock/rock.obj"), m_PlanetModel("src/Resources/planet/planet.obj"),
m_Amount(amount)
{
    SetUpBuffers();
}

InstancingExample::~InstancingExample()
{
}

void InstancingExample::Run(const glm::mat4& view, const glm::mat4& projection)
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    

    m_PlanetShader.Bind();
    m_PlanetShader.setMat4("projection", projection);
    m_PlanetShader.setMat4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
    model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
    m_PlanetShader.setMat4("model", model);

    m_PlanetModel.Draw(m_PlanetShader);

    m_AsteroidShader.Bind();
    m_AsteroidShader.setMat4("projection", projection);
    m_AsteroidShader.setMat4("view", view);

    m_RockModel.DrawInstanced(m_AsteroidShader, m_Amount);
}

void InstancingExample::SetUpTextures()
{
}

void InstancingExample::SetUpBuffers()
{
	std::vector<glm::mat4> modelMatrices(m_Amount);
	std::srand(static_cast<unsigned int>(glfwGetTime()));
	float radius = 150.0;
	float offset = 25.0f;

    for (unsigned int i = 0; i < m_Amount; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        // 1. translation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)m_Amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        // 2. scale: Scale between 0.05 and 0.25f
        float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
        model = glm::scale(model, glm::vec3(scale));

        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        float rotAngle = static_cast<float>((rand() % 360));
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. now add to list of matrices
        modelMatrices[i] = model;
    }

    m_InstancedVBO.SetData(modelMatrices.data(), m_Amount * sizeof(glm::mat4));
    m_InstancedVBO.SetLayout({ 
        //{ShaderDataType::Float3, "aPos"},
       // {ShaderDataType::Float2, "aTexCoords"},
        {ShaderDataType::Mat4, "aModelMatrix", false, 1} });

    m_RockModel.AddInstanceBuffer(m_InstancedVBO);
    
    

}

void InstancingExample::SetUpShaders()
{
}
