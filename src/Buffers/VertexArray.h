#pragma once

#include "VertexBuffer.h"

#include <stdint.h>

class VertexArray
{
public:
	VertexArray();

	void AddVertexBuffer(VertexBuffer VertexBuffer);

	void Bind();
	void UnBind();
	void Delete();

private:
	uint32_t m_ID;

};
