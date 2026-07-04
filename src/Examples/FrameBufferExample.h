#pragma once
#include "IExample.h"
#include "../Shader.h"
#include "../Buffers/FrameBuffer.h"
#include "../Buffers/VertexArray.h"
#include "../Buffers/VertexBuffer.h"
#include "../Camera.h"

class FrameBufferExample : public IExample
{
public:
	FrameBufferExample();
	~FrameBufferExample();



	void Run(const glm::mat4& view, const glm::mat4& projection) override;
	void Exercise(glm::mat4& view, glm::mat4& projection, Camera camera);


private:
	void SetUpTextures() override;
	void SetUpBuffers() override;
	void SetUpShaders() override;

private:
  
	Shader m_Shader;
	Shader m_ScreenShader;

	FrameBuffer m_FrameBuffer;

	VertexArray m_CubeVAO;
	VertexBuffer m_CubeVB;

	VertexArray m_PlaneVAO;
	VertexBuffer m_PlaneVB;

	VertexArray m_QuadVAO;
	VertexBuffer m_QuadVB;

	uint32_t m_CubeTexture;
	uint32_t m_FloorTexture;


};

