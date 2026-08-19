#pragma once

#include <vector>
#include "GameObject.h"

class GameLevel
{
public:
    std::vector<GameObject> Bricks;
    GameLevel() {}
    void Load(const char* file, uint32_t levelWidth, uint32_t levelHeight);
    void Draw(SpriteRenderer& renderer);
    bool IsCompleted();
private:
    void Init(const std::vector<std::vector<unsigned int>>& tileData,
        uint32_t levelWidth, uint32_t levelHeight);
};