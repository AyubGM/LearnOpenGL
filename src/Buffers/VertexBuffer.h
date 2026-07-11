#pragma once

#include "BufferLayout.h"

class VertexBuffer
{
public:
	VertexBuffer() = default;

	VertexBuffer(const void* vertices, uint32_t size);

	~VertexBuffer();

	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;
	VertexBuffer(VertexBuffer&& other) noexcept;
	VertexBuffer& operator=(VertexBuffer&& other) noexcept;

	void SetData(const void* vertices, uint32_t size);
	void Bind() const;
	void UnBind() const;
	void Delete();

	const BufferLayout& GetLayout() const { return m_Layout; }
	void SetLayout(const BufferLayout& layout) { m_Layout = layout; }


private:
	uint32_t m_ID = 0;
	BufferLayout m_Layout{};
	bool m_Initialized = false;
};


