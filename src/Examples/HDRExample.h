#pragma once

#include "IExample.h"
#include "../Shader.h"
#include "../Buffers/VertexArray.h"
#include "../Buffers/FrameBuffer.h"
#include "../Texture/Texture2D.h"
#include "../Camera.h"

class HDRExample : public IExample
{
public:
	HDRExample(uint32_t width, uint32_t height, Camera& camera);

	virtual void Run(const glm::mat4& view, const glm::mat4& projection) override;
	virtual ~HDRExample() {};

private:
	virtual void SetUpTextures() override;
	virtual void SetUpBuffers() override;
	virtual void SetUpShaders() override;

	void RenderQuad();
	void RenderCube();

private:
	Shader m_Shader;
	Shader m_HdrShader;
	VertexArray m_QuadVAO;
	VertexBuffer m_QuadVBO;

	VertexArray m_CubeVAO;
	VertexBuffer m_CubeVBO;

	Texture2D m_WoodTexture;

	FrameBuffer m_HdrFrameBuffer;

	std::vector<glm::vec3> m_LightPositions;
	std::vector<glm::vec3> m_LightColors;
	Camera& m_Camera;
	bool m_Hdr = true;
	float m_Exposure = 1.0f;

};