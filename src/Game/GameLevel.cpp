#include "GameLevel.h"
#include "ResourceManager.h"
#include <iostream>
#include <fstream>

void GameLevel::Load(const char* file, uint32_t levelWidth, uint32_t levelHeight)
{
    Bricks.clear();

    uint32_t tileCode;
    //GameLevel level;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<uint32_t>> tileData;
    if (fstream)
    {
        while (std::getline(fstream, line)) // read each line from level file
        {
            std::istringstream sstream(line);
            std::vector<uint32_t> row;
            while (sstream >> tileCode) // read each word separated by spaces
                row.push_back(tileCode);
            tileData.push_back(row);
        }
        if (tileData.size() > 0)
            Init(tileData, levelWidth, levelHeight);
    }
    else
    {
		std::cout << "Failed to load level file: " << file << std::endl;
    }
}

void GameLevel::Draw(SpriteRenderer& renderer)
{
    for (GameObject& tile : Bricks)
        if (!tile.Destroyed)
            tile.Draw(renderer);
}

bool GameLevel::IsCompleted()
{
    for (GameObject& tile : Bricks)
        if (!tile.IsSolid && !tile.Destroyed)
            return false;
    return true;
}


void GameLevel::Init(const std::vector<std::vector<uint32_t>>& tileData,
    uint32_t lvlWidth, uint32_t lvlHeight)
{
    // calculate dimensions
    uint32_t height = tileData.size();
    uint32_t width = tileData[0].size();
    float unit_width = lvlWidth / static_cast<float>(width);
    float unit_height = lvlHeight / height;
    // initialize level tiles based on tileData		
    for (uint32_t y = 0; y < height; ++y)
    {
        for (uint32_t x = 0; x < width; ++x)
        {
            // check block type from level data (2D level array)
            if (tileData[y][x] == 1) // solid
            {
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                GameObject obj(pos, size,
                    ResourceManager::GetTexture("block_solid"),
                    glm::vec3(0.8f, 0.8f, 0.7f)
                );
                obj.IsSolid = true;
                Bricks.push_back(obj);
            }
            else if (tileData[y][x] > 1)
            {
                glm::vec3 color = glm::vec3(1.0f); // original: white
                if (tileData[y][x] == 2)
                    color = glm::vec3(0.2f, 0.6f, 1.0f);
                else if (tileData[y][x] == 3)
                    color = glm::vec3(0.0f, 0.7f, 0.0f);
                else if (tileData[y][x] == 4)
                    color = glm::vec3(0.8f, 0.8f, 0.4f);
                else if (tileData[y][x] == 5)
                    color = glm::vec3(1.0f, 0.5f, 0.0f);

                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                Bricks.push_back(
                    GameObject(pos, size, ResourceManager::GetTexture("block"), color)
                );
            }
        }
    }
}