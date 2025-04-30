#pragma once
#ifndef VAO_H
#define VAO_H

#include<glad/glad.h>
#include"VBO.h"

class VAO
{
public:
	unsigned int ID;
	VAO();

	void LinkVBO(VBO VBO, unsigned int layout);

	void Bind();
	void UnBind();
	void Delete();
};
#endif // !VAO_H
