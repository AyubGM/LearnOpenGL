#include "Game.h"
#include "ResourceManager.h"
#include <glad/glad.h>
#include <Events/EventDispatcher.h>

const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const float PLAYER_VELOCITY(500.0f);

Game::Game(uint32_t width, uint32_t height) : m_State(GameState::ACTIVE), m_Width(width), m_Height(height)
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
	glm::mat4 g_OrthoProjection = glm::ortho(0.0f, static_cast<float>(m_Width), static_cast<float>(m_Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Bind();
	ResourceManager::GetShader("sprite").setInt("image", 0);
	ResourceManager::GetShader("sprite").setMat4("projection", g_OrthoProjection);
	m_SpriteRenderer = std::make_unique<SpriteRenderer>(ResourceManager::GetShader("sprite"));


    // load textures
    ResourceManager::LoadTexture("background", "assets/textures/background.jpg");
    ResourceManager::LoadTexture("face", "assets/textures/awesomeface.png");
    ResourceManager::LoadTexture("block", "assets/textures/block.png");
    ResourceManager::LoadTexture("block_solid", "assets/textures/block_solid.png");
    // load levels
    GameLevel one; one.Load("assets/levels/one.lvl", m_Width, m_Height / 2);
    GameLevel two; two.Load("assets/levels/two.lvl", m_Width, m_Height / 2);
    GameLevel three; three.Load("assets/levels/three.lvl", m_Width, m_Height / 2);
    GameLevel four; four.Load("assets/levels/four.lvl", m_Width, m_Height / 2);
    m_Levels.push_back(one);
    m_Levels.push_back(two);
    m_Levels.push_back(three);
    m_Levels.push_back(four);
    m_ActiveLevelIndex = 0;

	// load player


	ResourceManager::LoadTexture("paddle", "assets/textures/paddle.png");
	glm::vec2 playerPos = glm::vec2(
		m_Width / 2.0f - PLAYER_SIZE.x / 2.0f,
		m_Height - PLAYER_SIZE.y
	);
	m_Player = GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"), glm::vec3(1.0f));
}

void Game::Update(float dt)
{

}

void Game::ProcessInput(float dt)
{
	if (m_State == GameState::ACTIVE)
	{
		float velocity = PLAYER_VELOCITY * dt;
		if (Keys[GLFW_KEY_A])
		{
			if (m_Player.Position.x >= 0.0f)
				m_Player.Position.x -= velocity;
		}
		if (Keys[GLFW_KEY_D])
		{
			if (m_Player.Position.x <= m_Width - m_Player.Size.x)
				m_Player.Position.x += velocity;
		}
	}
}

void Game::Render()
{
	if (m_State == GameState::ACTIVE)
	{
		// draw background
		m_SpriteRenderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(m_Width, m_Height), 0.0f);
		// draw level
		m_Levels[m_ActiveLevelIndex].Draw(*m_SpriteRenderer);
		// draw player
		m_Player.Draw(*m_SpriteRenderer);
	}
	else if (m_State == GameState::MENU)
	{
		// draw menu
	}
	else if (m_State == GameState::WIN)
	{
		// draw win screen
	}

}
