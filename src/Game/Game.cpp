#include "Game.h"
#include "ResourceManager.h"
#include <glad/glad.h>

Game::Game(uint32_t width, uint32_t height) : State(GameState::ACTIVE), Width(width), Height(height)
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Game::~Game()
{
}

void Game::Init()
{
	ResourceManager::LoadShader("sprite", "assets/shaders/sprite.vert", "assets/shaders/sprite.frag");
	glm::mat4 g_OrthoProjection = glm::ortho(0.0f, static_cast<float>(Width), static_cast<float>(Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Bind();
	ResourceManager::GetShader("sprite").setInt("image", 0);
	ResourceManager::GetShader("sprite").setMat4("projection", g_OrthoProjection);
	m_SpriteRenderer = std::make_unique<SpriteRenderer>(ResourceManager::GetShader("sprite"));

	ResourceManager::LoadTexture("face", "assets/textures/awesomeface.png");
}

void Game::Update(float dt)
{

}

void Game::ProcessInput(float dt)
{

}

void Game::Render()
{
	m_SpriteRenderer->DrawSprite(ResourceManager::GetTexture("face"), glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}
