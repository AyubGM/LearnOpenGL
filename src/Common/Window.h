#pragma once

#include "../Camera.h"

#include <stdint.h>
#include <string>
#include <string_view>
#include <glm/glm.hpp>
class GLFWwindow;

class Window
{
public:
    Window(uint32_t width = 800, uint32_t height = 600, std::string_view name = "learnOpenGL", const Camera& camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f)));
	void OnCursorPos(double xpos, double ypos);
	void OnScroll(double yoffset);
	void OnFramebufferResize(int width, int height);
	void ProcessInput(float deltaTime);

	void OnUpdate();

	GLFWwindow* GetGLFWwindow() { return m_Window; }
	uint32_t GetWidth() { return m_Width; }
	uint32_t GetHeight() { return m_Height; }
	Camera& GetCamera() { return m_Camera; }
private:
	bool Init();
	

private:
	GLFWwindow* m_Window;
	uint32_t m_Width, m_Height;
	std::string m_Name;

	Camera m_Camera;
	bool m_FirstMove;
	

};