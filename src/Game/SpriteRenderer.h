#pragma once

#include <Buffers/VertexArray.h>
#include <Shader.h>
#include <Texture/Texture2D.h>

class SpriteRenderer
{
public:
	SpriteRenderer() = default;
	SpriteRenderer(Shader& shader);
	~SpriteRenderer();

	void DrawSprite(Texture2D& texture, glm::vec2 position,
		glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f,
		glm::vec3 color = glm::vec3(1.0f));

private:
	void Init();

private:
	Shader& m_Shader;
	VertexArray m_QuadVAO;
	VertexBuffer m_QuadVBO;
};

