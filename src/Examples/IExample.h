#pragma once
#include <glm/gtc/matrix_transform.hpp>

class IExample
{
public:
	virtual void SetUpTextures() = 0;
	virtual void SetUpBuffers() = 0;
	virtual void SetUpShaders() = 0;
	virtual void Run(const glm::mat4& view, const glm::mat4& projection) = 0;
	virtual ~IExample() {};

private:

};
