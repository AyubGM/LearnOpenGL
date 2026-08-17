#pragma once

#include <glad/glad.h>

#include <string>
#include <vector>
#include <cstdint>

class TextureCubeMap
{
public:
	TextureCubeMap() = default;
	TextureCubeMap(const std::vector<std::string>& path);
	~TextureCubeMap();

	// Rule of 5: Move semantics (prevent copy, allow move)
	TextureCubeMap(const TextureCubeMap&) = delete;
	TextureCubeMap& operator=(const TextureCubeMap&) = delete;
	TextureCubeMap(TextureCubeMap&& other) noexcept;
	TextureCubeMap& operator=(TextureCubeMap&& other) noexcept;

	void AllocateEmpty(uint32_t width, uint32_t height, GLenum internalFormat = GL_RGB16F, bool enableMipmaps = true);

	void Bind(uint32_t slot = 0) const;
	void UnBind(uint32_t slot = 0) const;
	void Delete();

	inline uint32_t GetID() const { return m_ID; }
	inline uint32_t GetWidth() const { return m_Width; }
	inline uint32_t GetHeight() const { return m_Height; }
	inline const std::vector<std::string>& GetPath() const { return m_Paths; }


private:
	uint32_t m_ID = 0;
	uint32_t m_Width = 0;
	uint32_t m_Height = 0;
	std::vector<std::string> m_Paths;
};