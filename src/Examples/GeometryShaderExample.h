#pragma once
#include "IExample.h"
#include "../Shader.h"
#include "../Buffers/VertexArray.h"
#include "../Buffers/VertexBuffer.h"
#include "../Model.h"


class GeometryShaderExample : public IExample
{
public:
	GeometryShaderExample();
	~GeometryShaderExample();

	void Run(const glm::mat4& view, const glm::mat4& projection) override;

private:
	void SetUpTextures() override;
	void SetUpBuffers() override;
	void SetUpShaders() override;

private:

	Shader m_Shader;
	Model m_BackPack;


};

