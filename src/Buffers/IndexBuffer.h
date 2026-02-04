#pragma once

#include <stdint.h>

class IndexBuffer
{
public:

	IndexBuffer(uint32_t* indices, uint32_t count);

	void Bind();
	void UnBind();
	void Delete();

private:
	uint32_t m_ID;

};

