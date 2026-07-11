#pragma once
#include "IExample.h"
#include "../Shader.h"
#include "../Buffers/VertexArray.h"
#include "../Buffers/VertexBuffer.h"
#include "../Geometry/Model.h"

class InstancingExample : public IExample
{
public:
	InstancingExample(uint32_t amount = 10000);
	~InstancingExample();

	void Run(const glm::mat4& view, const glm::mat4& projection) override;

private:
	void SetUpTextures() override;
	void SetUpBuffers() override;
	void SetUpShaders() override;

private:
	Shader m_AsteroidShader;
	Shader m_PlanetShader;
	Model m_RockModel;
	Model m_PlanetModel;

	VertexBuffer m_InstancedVBO;
	uint32_t m_Amount;


};
