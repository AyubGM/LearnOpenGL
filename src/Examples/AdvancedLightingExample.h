#pragma once

#include "IExample.h"
#include "../Shader.h"
#include "../Buffers/VertexArray.h"
#include "../Texture/Texture2D.h"
#include "../Camera.h"


class AdvancedLightingExample : public IExample
{
public:
	AdvancedLightingExample(Camera& camera);
	~AdvancedLightingExample();

	void Run(const glm::mat4& view, const glm::mat4& projection) override;

private:


	// Inherited via IExample
	void SetUpTextures() override;

	void SetUpBuffers() override;

	void SetUpShaders() override;

private:
	Shader m_Shader;

	VertexArray m_PlaneVAO;
	VertexBuffer m_PlaneVBO;

	Texture2D m_FloorTexture;
	bool m_Blinn = true;
	Camera& m_Camera;



};

