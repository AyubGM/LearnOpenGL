#pragma once
#include "IExample.h"
#include "../Shader.h"
#include "../Buffers/VertexArray.h"
#include "../Buffers/FrameBuffer.h"

class AntiAliasingExample : public IExample
{
public:
	AntiAliasingExample(uint32_t& width, uint32_t& height);
	~AntiAliasingExample();

	void Run(const glm::mat4& view, const glm::mat4& projection) override;

private:
	void SetUpTextures() override;
	void SetUpBuffers() override;
	void SetUpShaders() override;


private:
	Shader m_Shader;
	Shader m_ScreenShader;

	VertexArray m_CubeVAO;
	VertexBuffer m_CubeVBO;

	VertexArray m_QuadVAO;
	VertexBuffer m_QuadVBO;

	FrameBuffer m_MsaaFrameBuffer;
	FrameBuffer m_IntermediateFrameBuffer;

	uint32_t& m_Width;
	uint32_t& m_Height;

};

