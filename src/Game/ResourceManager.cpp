#include "ResourceManager.h"
#include <iostream>

std::unordered_map<std::string, Shader>    ResourceManager::s_Shaders;
std::unordered_map<std::string, Texture2D> ResourceManager::s_Textures;

Shader& ResourceManager::LoadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
{
    auto [it, inserted] = s_Shaders.emplace(name, Shader(vertexPath.c_str(), fragmentPath.c_str()));
    return it->second;
}

Shader& ResourceManager::GetShader(const std::string& name)
{
    auto it = s_Shaders.find(name);
    if (it != s_Shaders.end())
    {
        return it->second;
    }

    std::cerr << "Warning: Shader '" << name << "' not found!\n";
    static Shader nullShader;
    return nullShader;
}

Texture2D& ResourceManager::LoadTexture(const std::string& name, const std::string& path)
{
    auto [it, inserted] = s_Textures.emplace(name, Texture2D(path));
    return it->second;
}

Texture2D& ResourceManager::GetTexture(const std::string& name)
{
    auto it = s_Textures.find(name);
    if (it != s_Textures.end())
    {
        return it->second;
    }

    std::cerr << "Warning: Texture '" << name << "' not found!\n";
    static Texture2D nullTexture;
    return nullTexture;
}

void ResourceManager::Clear()
{
    for (auto& [name, shader] : s_Shaders)
    {
        shader.Delete();
    }
    s_Shaders.clear();

    for (auto& [name, texture] : s_Textures)
    {
        texture.Delete();
    }
    s_Textures.clear();
}