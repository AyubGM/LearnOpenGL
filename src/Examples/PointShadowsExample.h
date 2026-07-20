#pragma once
#include "IExample.h"
#include "../Buffers/VertexArray.h"
#include "../Buffers/FrameBuffer.h"
#include "../Shader.h"
#include "../Texture/Texture2D.h"
#include "../Camera.h"

class PointShadowsExample : public IExample
{
public:
	PointShadowsExample(uint32_t& width, uint32_t& height, Camera& camera);
	~PointShadowsExample();

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
	Shader m_SimpleDepthShader;

	VertexArray m_PlaneVAO;
	VertexBuffer m_PlaneVBO;

	VertexArray m_QuadVAO;
	VertexBuffer m_QuadVBO;

	VertexArray m_CubeVAO;
	VertexBuffer m_CubeVBO;


	FrameBuffer m_DepthMapFBO;
	Texture2D m_WoodTexture;
	uint32_t& m_Width;
	uint32_t& m_Height;

	glm::vec3 m_LightPos;
	Camera& m_Camera;

};

