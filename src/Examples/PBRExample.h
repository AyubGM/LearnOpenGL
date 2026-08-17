#pragma once

#include "IExample.h"


#include "../Buffers/FrameBuffer.h"
#include "../Buffers/VertexArray.h"
#include "../Shader.h"
#include "../Camera.h"
#include "../Geometry/Model.h"

#include <vector>

class PBRExample : public IExample
{
public:
	PBRExample(Camera& camera);
	~PBRExample();

	void Run(const glm::mat4& view, const glm::mat4& projection) override;

private:
	// Inherited via IExample
	void SetUpTextures() override;

	void SetUpBuffers() override;

	void SetUpShaders() override;

	void RenderSphere();

private:
	Camera& m_Camera;

	Shader m_Shader;

	VertexArray m_SphereVAO;
	VertexBuffer m_SphereVBO;
	IndexBuffer m_SphereEBO;

	std::vector<glm::vec3> m_LightPositions;
	std::vector<glm::vec3> m_LightColors;

	int m_NrRows = 7;
	int m_NrColumns = 7;
	float m_Spacing = 2.5;

};