#pragma once

#include <string>
#include <cstdint>

class Texture2D
{
public:
	Texture2D() = default;
	Texture2D(const std::string& path, const std::string& type = "texture_diffuse");
	~Texture2D();

	// Rule of 5: Move semantics (prevent copy, allow move)
	Texture2D(const Texture2D&) = delete;
	Texture2D& operator=(const Texture2D&) = delete;
	Texture2D(Texture2D&& other) noexcept;
	Texture2D& operator=(Texture2D&& other) noexcept;

	void Bind(uint32_t slot = 0) const;
	void UnBind(uint32_t slot = 0) const;
	void Delete();

	inline uint32_t GetID() const { return m_ID; }
	inline uint32_t GetWidth() const { return m_Width; }
	inline uint32_t GetHeight() const { return m_Height; }
	inline const std::string& GetType() const { return m_Type; }
	inline const std::string& GetPath() const { return m_Path; }

	void SetType(const std::string& type) { m_Type = type; }

private:
	uint32_t m_ID = 0;
	uint32_t m_Width = 0;
	uint32_t m_Height = 0;
	std::string m_Path;
	std::string m_Type;
};