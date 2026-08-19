#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "SpriteRenderer.h"
#include <memory>
#include "GameLevel.h"
#include "GameObject.h"
#include "BallObject.h"

enum class GameState
{
    ACTIVE,
    MENU,
    WIN
};

class Game
{
public:
   
    // constructor/destructor
    Game(uint32_t width, uint32_t height);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();

    void DoCollisions();

private:
    bool CheckCollision(GameObject& one, GameObject& two);
    bool CheckCollision(BallObject& one, GameObject& two);

public:
    // game state
    GameState               m_State;
    bool                    Keys[1024]{false};
    uint32_t            m_Width, m_Height;

private:
    std::unique_ptr<SpriteRenderer> m_SpriteRenderer;
    std::vector<GameLevel> m_Levels;
    uint32_t m_ActiveLevelIndex;



	GameObject m_Player;
	BallObject m_Ball;

};

