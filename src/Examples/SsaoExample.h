#pragma once

#include "IExample.h"


#include "../Buffers/FrameBuffer.h"
#include "../Buffers/VertexArray.h"
#include "../Shader.h"
#include "../Camera.h"
#include "../Geometry/Model.h"

#include <vector>

class SsaoExample : public IExample
{
public:
	SsaoExample(uint32_t& width, uint32_t& height, Camera& camera);
	~SsaoExample();

	void Run(const glm::mat4& view, const glm::mat4& projection) override;

private:
	// Inherited via IExample
	void SetUpTextures() override;

	void SetUpBuffers() override;

	void SetUpShaders() override;

	void RenderQuad();
	void RenderCube();

	float Lerp(float a, float b, float f);

private:
	uint32_t& m_Width;
	uint32_t& m_Height;
	Camera& m_Camera;

	Shader m_ShaderGeometryPass;
	Shader m_ShaderLightingPass;
	Shader m_ShaderSSAO;
	Shader m_ShaderSSAOBlur;

	VertexArray m_QuadVAO;
	VertexBuffer m_QuadVBO;

	VertexArray m_CubeVAO;
	VertexBuffer m_CubeVBO;

	FrameBuffer m_GBuffer;
	FrameBuffer m_SsaoFBO;
	FrameBuffer m_SsaoBlurFBO;

	Model m_Backpack;


	std::vector<glm::vec3> m_ObjectPositions;
	glm::vec3 m_LightPosition;
	glm::vec3 m_LightColor;
	std::vector<glm::vec3> m_SsaoKernel;
	std::vector<glm::vec3> m_SsaoNoise;

	uint32_t m_NRLIGHTS = 32;

	Texture2D m_NoiseTexture;
};