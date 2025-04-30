#pragma once
#ifndef  EBO_H
#define EBO_H

#include<glad/glad.h>

class EBO
{
public:
	unsigned int ID;
	EBO(unsigned int* indices, GLsizeiptr size);

	void Bind();
	void UnBind();
	void Delete();

private:

};


#endif // ! EBO_H
