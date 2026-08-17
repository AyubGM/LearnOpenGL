#include "PBRExample.h"
#include <GLFW/glfw3.h>

PBRExample::PBRExample(Camera& camera) : m_Shader("Shaders/PBR/1.1.pbr.vert", "Shaders/PBR/1.1.pbr.frag"),
	m_Camera(camera)
{
	SetUpTextures();
	SetUpBuffers();
	SetUpShaders();

	m_LightPositions = {
		glm::vec3(-10.0f,  10.0f, 10.0f),
		glm::vec3(10.0f,  10.0f, 10.0f),
		glm::vec3(-10.0f, -10.0f, 10.0f),
		glm::vec3(10.0f, -10.0f, 10.0f),
	};

	m_LightColors = {
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f)
	};
}

PBRExample::~PBRExample()
{
}

void PBRExample::Run(const glm::mat4& view, const glm::mat4& projection)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_Shader.Bind();
	m_Shader.setMat4("projection", projection);
	m_Shader.setMat4("view", view);
	m_Shader.setVec3("camPos", m_Camera.Position);

	// render rows*column number of spheres with varying metallic/roughness values scaled by rows and columns respectively
	glm::mat4 model = glm::mat4(1.0f);
	for (int row = 0; row < m_NrRows; ++row)
	{
		m_Shader.setFloat("metallic", (float)row / (float)m_NrRows);
		for (int col = 0; col < m_NrColumns; ++col)
		{
			// we clamp the roughness to 0.05 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
			// on direct lighting.
			m_Shader.setFloat("roughness", glm::clamp((float)col / (float)m_NrColumns, 0.05f, 1.0f));

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(
				(col - (m_NrColumns / 2)) * m_Spacing,
				(row - (m_NrRows / 2)) * m_Spacing,
				0.0f
			));
			m_Shader.setMat4("model", model);
			m_Shader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
			RenderSphere();
		}
	}

	// render light source (simply re-render sphere at light positions)
	// this looks a bit off as we use the same shader, but it'll make their positions obvious and 
	// keeps the codeprint small.
	for (unsigned int i = 0; i < m_LightPositions.size(); ++i)
	{
		glm::vec3 newPos = m_LightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
		//newPos = m_LightPositions[i];
		m_Shader.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
		m_Shader.setVec3("lightColors[" + std::to_string(i) + "]", m_LightColors[i]);

		model = glm::mat4(1.0f);
		model = glm::translate(model, newPos);
		model = glm::scale(model, glm::vec3(0.5f));
		m_Shader.setMat4("model", model);
		m_Shader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
		RenderSphere();
	}

}

void PBRExample::SetUpTextures()
{
}

void PBRExample::SetUpBuffers()
{

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

	const unsigned int X_SEGMENTS = 64;
	const unsigned int Y_SEGMENTS = 64;
	const float PI = 3.14159265359f;
	for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
	{
		for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
		{
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

			positions.push_back(glm::vec3(xPos, yPos, zPos));
			uv.push_back(glm::vec2(xSegment, ySegment));
			normals.push_back(glm::vec3(xPos, yPos, zPos));
		}
	}

	bool oddRow = false;
	for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
	{
		if (!oddRow) // even rows: y == 0, y == 2; and so on
		{
			for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
			{
				indices.push_back(y * (X_SEGMENTS + 1) + x);
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
			}
		}
		else
		{
			for (int x = X_SEGMENTS; x >= 0; --x)
			{
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				indices.push_back(y * (X_SEGMENTS + 1) + x);
			}
		}
		oddRow = !oddRow;
	}

	std::vector<float> data;
	for (unsigned int i = 0; i < positions.size(); ++i)
	{
		data.push_back(positions[i].x);
		data.push_back(positions[i].y);
		data.push_back(positions[i].z);
		if (normals.size() > 0)
		{
			data.push_back(normals[i].x);
			data.push_back(normals[i].y);
			data.push_back(normals[i].z);
		}
		if (uv.size() > 0)
		{
			data.push_back(uv[i].x);
			data.push_back(uv[i].y);
		}
	}

	m_SphereVBO.SetData(data.data(), data.size() * sizeof(float));
	m_SphereVBO.SetLayout({
		{ShaderDataType::Float3, "aPos"},
		{ShaderDataType::Float3, "aNormal"},
		{ShaderDataType::Float2, "aTexCoords"}
		});

	m_SphereEBO.SetData(indices.data(), indices.size());

	m_SphereVAO.AddVertexBuffer(m_SphereVBO);
	m_SphereVAO.SetIndexBuffer(m_SphereEBO);
}

void PBRExample::SetUpShaders()
{
	m_Shader.setVec3("albedo", { 0.5f, 0.0f, 0.0f });
	m_Shader.setFloat("ao", 1.0f);
}

void PBRExample::RenderSphere()
{
	m_SphereVAO.Bind();
	glDrawElements(GL_TRIANGLE_STRIP, m_SphereEBO.GetCount(), GL_UNSIGNED_INT, 0);
}
