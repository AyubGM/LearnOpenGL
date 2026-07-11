#pragma once

#include <stdint.h>

class IndexBuffer
{
public:

	IndexBuffer() = default;
	IndexBuffer(const uint32_t* indices, uint32_t count);
	~IndexBuffer();

	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer& operator=(const IndexBuffer&) = delete;
	IndexBuffer(IndexBuffer&& other) noexcept;
	IndexBuffer& operator=(IndexBuffer&& other) noexcept;


	void Bind() const;
	void UnBind() const;
	void Delete();

	uint32_t GetCount() const { return m_Count; }


private:
	uint32_t m_ID = 0;
	uint32_t m_Count = 0;

};

