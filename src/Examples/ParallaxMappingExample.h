#pragma once
#include "IExample.h"
#include "../Shader.h"
#include "../Texture/Texture2D.h"
#include "../Camera.h"
#include "../Buffers/VertexArray.h"

class ParallaxMappingExample : public IExample
{
public:
	ParallaxMappingExample(Camera& camera);
	~ParallaxMappingExample();

	void Run(const glm::mat4& view, const glm::mat4& projection) override;

private:


	// Inherited via IExample
	void SetUpTextures() override;

	void SetUpBuffers() override;

	void SetUpShaders() override;

	void RenderQuad();

private:
	Shader m_Shader;
	Texture2D m_DiffuseMap;
	Texture2D m_NormalMap;
	Texture2D m_HeightMap;

	VertexArray m_QuadVAO;
	VertexBuffer m_QuadVBO;

	glm::vec3 m_LightPos;
	Camera& m_Camera;
	float m_HeightScale = 0.1f;

};

