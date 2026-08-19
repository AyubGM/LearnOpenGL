#include "Game.h"
#include "ResourceManager.h"
#include <glad/glad.h>
#include <Events/EventDispatcher.h>

const glm::vec2 PLAYER_SIZE(100.0f, 20.0f);
const float PLAYER_VELOCITY(500.0f);

// Initial velocity of the Ball
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// Radius of the ball object
const float BALL_RADIUS = 12.5f;

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


	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS,
		-BALL_RADIUS * 2.0f);
	m_Ball = BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,
		ResourceManager::GetTexture("face"));
}

void Game::Update(float dt)
{
	m_Ball.Move(dt, m_Width);
	DoCollisions();
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

			if (m_Ball.Stuck)
				m_Ball.Position.x -= velocity;
		}
		if (Keys[GLFW_KEY_D])
		{
			if (m_Player.Position.x <= m_Width - m_Player.Size.x)
				m_Player.Position.x += velocity;

			if (m_Ball.Stuck)
				m_Ball.Position.x += velocity;
		}

		if (Keys[GLFW_KEY_SPACE])
		{
			m_Ball.Stuck = false;
		}
	}
}

void Game::DoCollisions()
{
	for (GameObject& box : m_Levels[m_ActiveLevelIndex].Bricks)
	{
		if (!box.Destroyed)
		{
			if (CheckCollision(m_Ball, box))
			{
				if (!box.IsSolid)
					box.Destroyed = true;
			}
		}
	}
}

bool Game::CheckCollision(GameObject& one, GameObject& two) // AABB - AABB collision
{
	// collision x-axis?
	bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
		two.Position.x + two.Size.x >= one.Position.x;
	// collision y-axis?
	bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
		two.Position.y + two.Size.y >= one.Position.y;
	// collision only if on both axes
	return collisionX && collisionY;
}

bool Game::CheckCollision(BallObject& one, GameObject& two) // AABB - Circle collision
{
	// get center point circle first 
	glm::vec2 center(one.Position + one.Radius);
	// calculate AABB info (center, half-extents)
	glm::vec2 aabb_half_extents(two.Size.x / 2.0f, two.Size.y / 2.0f);
	glm::vec2 aabb_center(
		two.Position.x + aabb_half_extents.x,
		two.Position.y + aabb_half_extents.y
	);
	// get difference vector between both centers
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	// add clamped value to AABB_center and we get the value of box closest to circle
	glm::vec2 closest = aabb_center + clamped;
	// retrieve vector between center circle and closest point AABB and check if length <= radius
	difference = closest - center;
	return glm::length(difference) < one.Radius;
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
		// draw ball
		m_Ball.Draw(*m_SpriteRenderer);
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
