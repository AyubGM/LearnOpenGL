#pragma once

#include "IExample.h"
#include "../Buffers/FrameBuffer.h"
#include "../Buffers/VertexArray.h"
#include "../Shader.h"
#include "../Texture/Texture2D.h"
#include "../Camera.h"


class BloomExample : public IExample
{
public:
	BloomExample(uint32_t& width, uint32_t& height, Camera& camera);
	~BloomExample();

	void Run(const glm::mat4& view, const glm::mat4& projection) override;

private:
	// Inherited via IExample
	void SetUpTextures() override;

	void SetUpBuffers() override;

	void SetUpShaders() override;

	void RenderQuad();
	void RenderCube();


private:
	uint32_t& m_Width;
	uint32_t& m_Height;
	Camera& m_Camera;

	bool m_Bloom = true;
	bool m_BloomKeyPressed = false;
	float m_Exposure = 1.0f;

	VertexArray m_QuadVAO;
	VertexBuffer m_QuadVBO;

	VertexArray m_CubeVAO;
	VertexBuffer m_CubeVBO;

	Shader m_Shader;
	Shader m_ShaderLight;
	Shader m_ShaderBlur;
	Shader m_ShaderBloomFinal;

	Texture2D m_WoodTexture;
	Texture2D m_ContainerTexture;

	FrameBuffer m_HdrFBO;
	FrameBuffer m_PingpongFBO1;
	FrameBuffer m_PingpongFBO2;

	std::vector<glm::vec3> m_LightPositions;
	std::vector<glm::vec3> m_LightColors;


};


