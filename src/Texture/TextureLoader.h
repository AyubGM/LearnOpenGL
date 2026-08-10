#pragma once
#include <cstdint>
#include <string>
#include <vector>


class TextureLoader
{
public:
	static uint32_t LoadTexture(char const* path);
	static uint32_t LoadCubemap(std::vector<std::string> faces);

private:
	TextureLoader() = delete;

};

