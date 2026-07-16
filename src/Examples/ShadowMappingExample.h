#pragma once
#include "IExample.h"
#include "../Buffers/VertexArray.h"
#include "../Buffers/FrameBuffer.h"
#include "../Shader.h"
#include "../Texture/Texture2D.h"

class ShadowMappingExample : public IExample
{
public:
	ShadowMappingExample(uint32_t& width, uint32_t& height);
	~ShadowMappingExample();

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
	Shader m_DebugDepthQuadShader;

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

};

