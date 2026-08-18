#include "SpriteRenderer.h"
#include <print>

SpriteRenderer::SpriteRenderer(Shader& shader)
	: m_Shader(shader)
{
    Init();
}

SpriteRenderer::~SpriteRenderer()
{
	m_QuadVAO.Delete();
	m_QuadVBO.Delete();
}

void SpriteRenderer::Init()
{
    std::println("Initializing SpriteRenderer...");
    float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

	m_QuadVBO.SetData(vertices, sizeof(vertices));
	m_QuadVBO.SetLayout({
		{ ShaderDataType::Float4, "vertex" },
		});

	m_QuadVAO.AddVertexBuffer(m_QuadVBO);
}

void SpriteRenderer::DrawSprite(Texture2D& texture, glm::vec2 position,
    glm::vec2 size, float rotate, glm::vec3 color)
{
    // prepare transformations
    m_Shader.Bind();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size, 1.0f));

    m_Shader.setMat4("model", model);
    m_Shader.setVec3("spriteColor", color);

    texture.Bind();

    m_QuadVAO.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
}