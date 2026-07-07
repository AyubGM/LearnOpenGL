#pragma once

#include "IExample.h"
#include "../Shader.h"
#include "../Buffers/FrameBuffer.h"
#include "../Buffers/VertexArray.h"
#include "../Buffers/VertexBuffer.h"
#include "../Camera.h"
#include "../Buffers/UniformBuffer.h"

class UniformBufferExample : public IExample
{
public:
	UniformBufferExample();
	~UniformBufferExample();



	void Run(const glm::mat4& view, const glm::mat4& projection) override;


private:
	void SetUpTextures() override;
	void SetUpBuffers() override;
	void SetUpShaders() override;

private:

	Shader m_ShaderRed;
	Shader m_ShaderGreen;
	Shader m_ShaderBlue;
	Shader m_ShaderYellow;

	VertexArray m_CubeVAO;
	VertexBuffer m_CubeVB;

	
	UniformBuffer m_UboMatrices;

};