#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "SpriteRenderer.h"
#include <memory>

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

public:
    // game state
    GameState               State;
    bool                    Keys[1024];
    uint32_t            Width, Height;

private:
    std::unique_ptr<SpriteRenderer> m_SpriteRenderer;

};

