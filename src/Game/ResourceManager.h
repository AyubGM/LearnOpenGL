#pragma once

#include "../Texture/Texture2D.h"
#include "../Shader.h"
#include <string>
#include <unordered_map>
#include <memory>

class ResourceManager
{
public:
    ResourceManager() = delete;
    ~ResourceManager() = delete;

    static Shader& LoadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
    static Shader& GetShader(const std::string& name);

    static Texture2D& LoadTexture(const std::string& name, const std::string& path);
    static Texture2D& GetTexture(const std::string& name);

    static void Clear();

private:
    static std::unordered_map<std::string, Shader> s_Shaders;
    static std::unordered_map<std::string, Texture2D> s_Textures;
};