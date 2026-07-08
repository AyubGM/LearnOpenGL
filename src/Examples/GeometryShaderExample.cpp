#include "GeometryShaderExample.h"
#include <GLFW/glfw3.h>


GeometryShaderExample::GeometryShaderExample() : m_ExplodeShader("src/shaders/GeometryShader/9.2.geometry_shader.vert",
	"src/shaders/GeometryShader/9.2.geometry_shader.frag", "src/shaders/GeometryShader/9.2.geometry_shader.geom"),

	m_Shader("src/shaders/GeometryShader/9.3.default.vert", "src/shaders/GeometryShader/9.3.default.frag"),

	m_NormalShader("src/shaders/GeometryShader/9.3.normal_visualization.vert",
		"src/shaders/GeometryShader/9.3.normal_visualization.frag", "src/shaders/GeometryShader/9.3.normal_visualization.geom"),

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
	m_ExplodeShader.use();
	m_ExplodeShader.setMat4("projection", projection);
	m_ExplodeShader.setMat4("view", view);
	m_ExplodeShader.setMat4("model", model);
	m_ExplodeShader.setFloat("time", static_cast<float>(glfwGetTime()));
	m_BackPack.Draw(m_ExplodeShader);
}

void GeometryShaderExample::VisualizeNormals(const glm::mat4& view, const glm::mat4& projection)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 model = glm::mat4(1.0f);
	m_Shader.use();
	m_Shader.setMat4("projection", projection);
	m_Shader.setMat4("view", view);
	m_Shader.setMat4("model", model);

	m_BackPack.Draw(m_Shader);

	m_NormalShader.use();
	m_NormalShader.setMat4("projection", projection);
	m_NormalShader.setMat4("view", view);
	m_NormalShader.setMat4("model", model);

	m_BackPack.Draw(m_NormalShader);
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
