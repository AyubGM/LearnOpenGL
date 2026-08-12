#pragma once

#include "IExample.h"
#include "../Buffers/FrameBuffer.h"
#include "../Buffers/VertexArray.h"
#include "../Shader.h"
#include "../Camera.h"
#include "../Geometry/Model.h"

#include <vector>

class DeferredShadingExample : public IExample
{
public:
	DeferredShadingExample(uint32_t& width, uint32_t& height, Camera& camera);
	~DeferredShadingExample();

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


	Shader m_ShaderGeometryPass;
	Shader m_ShaderLightingPass;
	Shader m_ShaderLightBox;

	VertexArray m_QuadVAO;
	VertexBuffer m_QuadVBO;

	VertexArray m_CubeVAO;
	VertexBuffer m_CubeVBO;

	FrameBuffer m_GBuffer;

	Model m_Backpack;


	std::vector<glm::vec3> m_ObjectPositions;
	std::vector<glm::vec3> m_LightPositions;
	std::vector<glm::vec3> m_LightColors;

	uint32_t m_NRLIGHTS = 32;

	//Debug
	Texture2D m_ContainerTexture;
	Texture2D m_ContainerTextureSpec;
};

