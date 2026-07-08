#include "GeometryShaderExample.h"
#include <GLFW/glfw3.h>


GeometryShaderExample::GeometryShaderExample() : m_Shader("src/shaders/GeometryShader/9.2.geometry_shader.vert", 
	"src/shaders/GeometryShader/9.2.geometry_shader.frag", "src/shaders/GeometryShader/9.2.geometry_shader.geom"),
	m_BackPack("src/Resources/backpack/backpack.obj")
{
	SetUpTextures();
	SetUpBuffers();
	SetUpShaders();
}

GeometryShaderExample::~GeometryShaderExample()
{
}

void GeometryShaderExample::Run(const glm::mat4& view, const glm::mat4& projection)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 model = glm::mat4(1.0f);
	m_Shader.use();
	m_Shader.setMat4("projection", projection);
	m_Shader.setMat4("view", view);
	m_Shader.setMat4("model", model);
	m_Shader.setFloat("time", static_cast<float>(glfwGetTime()));
	m_BackPack.Draw(m_Shader);
}

void GeometryShaderExample::SetUpTextures()
{
}

void GeometryShaderExample::SetUpBuffers()
{
}

void GeometryShaderExample::SetUpShaders()
{
}
