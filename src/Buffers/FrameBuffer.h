#pragma once
#include <cstdint>
class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	void Bind() const;
	void UnBind() const;

	uint32_t GetTextureColorBuffer() const { return m_TextureColorBuffer; }

private:
	uint32_t m_ID;
	uint32_t m_TextureColorBuffer;
	uint32_t m_Rbo;
};

