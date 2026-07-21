#pragma once
#include "IExample.h"
#include "../Buffers/VertexArray.h"
#include "../Buffers/FrameBuffer.h"
#include "../Shader.h"
#include "../Texture/Texture2D.h"
#include "../Camera.h"

class NormalMappingExample : public IExample
{
public:
	NormalMappingExample(uint32_t& width, uint32_t& height, Camera& camera);
	~NormalMappingExample();

	void Run(const glm::mat4& view, const glm::mat4& projection) override;
	void ShadowsRun(const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos);


private:


	// Inherited via IExample
	void SetUpTextures() override;

	void SetUpBuffers() override;

	void SetUpShaders() override;

	void RenderScene(const Shader& shader);
	void RenderQuad();
	void RenderCube();

private:
	Shader m_Shader;
	VertexArray m_QuadVAO;
	VertexBuffer m_QuadVBO;

	Texture2D m_DiffuseMap;
	Texture2D m_NormalMap;

	glm::vec3 m_LightPos;
	Camera& m_Camera;

};