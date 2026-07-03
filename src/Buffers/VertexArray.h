#pragma once

#include "VertexBuffer.h"

#include <stdint.h>

class VertexArray
{
public:
	VertexArray();

	void AddVertexBuffer(const VertexBuffer& VertexBuffer) const;

	void Bind();
	void UnBind();
	void Delete();

private:
	uint32_t m_ID;

};
