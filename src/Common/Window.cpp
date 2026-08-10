#include "Window.h"
#include <GLFW/glfw3.h>
#include <print>
#include "../Events/EventDispatcher.h"

namespace Utils
{
    static void FramebufferSizeCallbackFn(GLFWwindow* window, int width, int height)
    {
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

        if (win)
            win->OnFramebufferResize(width, height);

		g_EventDispatcher.DispatchEvent<FramebufferResizeEvent>(FramebufferResizeEvent{ width, height });
    }

    static void CursorPositionCallbackFn(GLFWwindow* window, double xposIn, double yposIn)
    {
        // Retrieve the Window instance from the user pointer
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (win)
            win->OnCursorPos(xposIn, yposIn);


    }

    static void ScrollCallbackFn(GLFWwindow* window, double xOffset, double yOffset)
    {
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (win)
            win->OnScroll(yOffset);

		g_EventDispatcher.DispatchEvent<MouseScrollEvent>(MouseScrollEvent{ xOffset, yOffset });
    }

    static void KeyCallbackFn(GLFWwindow* window, int key, int scancode, int action, int mods)
    {

      g_EventDispatcher.DispatchEvent<KeyEvent>(KeyEvent{ key, action, mods });

    }
}

Window::Window(uint32_t width, uint32_t height, std::string_view name) : 
    m_Width(width), m_Height(height), m_Name(name), m_FirstMove(true)
{
    Init();
}

bool Window::Init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(m_Width, m_Height, m_Name.c_str(), NULL, NULL);

    if (m_Window == NULL)
    {
        std::println("Failed to create GLFW window");
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(m_Window);

    glfwSetWindowUserPointer(m_Window, this);

    glfwSetFramebufferSizeCallback(m_Window, Utils::FramebufferSizeCallbackFn);
    glfwSetCursorPosCallback(m_Window, Utils::CursorPositionCallbackFn);
    glfwSetScrollCallback(m_Window, Utils::ScrollCallbackFn);
	glfwSetKeyCallback(m_Window, Utils::KeyCallbackFn);
    // tell GLFW to capture our mouse
    //glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return true;

}

void Window::OnUpdate()
{
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}

void Window::OnCursorPos(double xpos, double ypos)
{
    static double lastX = m_Width / 2.0;
    static double lastY = m_Height / 2.0;

    if (m_FirstMove)
    {
        lastX = xpos;
        lastY = ypos;
        m_FirstMove = false;
    }

    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos; // Reversed: y ranges bottom to top

    lastX = xpos;
    lastY = ypos;

    if (m_WrapCursor)
    {
        if (xpos <= 0) {
            glfwSetCursorPos(m_Window, m_Width - 2, ypos);
            lastX = m_Width - 2;
        }
        else if (xpos >= m_Width - 1) {
            glfwSetCursorPos(m_Window, 1, ypos);
            lastX = 1;
        }

        if (ypos <= 0) {
            glfwSetCursorPos(m_Window, xpos, m_Height - 2);
            lastY = m_Height - 2;
        }
        else if (ypos >= m_Height - 1) {
            glfwSetCursorPos(m_Window, xpos, 1);
            lastY = 1;
        }
    }
    g_EventDispatcher.DispatchEvent<MouseMoveEvent>(MouseMoveEvent{ xoffset, yoffset });
}

void Window::OnScroll(double yoffset)
{
}

void Window::OnFramebufferResize(int width, int height)
{
    // update stored size and OpenGL viewport
    m_Width = static_cast<uint32_t>(width);
    m_Height = static_cast<uint32_t>(height);
    glViewport(0, 0, width, height);
}


void Window::ProcessInput(float deltaTime)
{
    if (!m_Window) return;

    if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_Window, true);

    if (glfwGetKey(m_Window, GLFW_KEY_TAB) == GLFW_PRESS)
        m_WrapCursor = !m_WrapCursor;

    if (glfwGetKey(m_Window, GLFW_KEY_F) == GLFW_PRESS && !m_FocusMode) {
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        m_FocusMode = true;
    }
    else if (glfwGetKey(m_Window, GLFW_KEY_F) == GLFW_RELEASE && m_FocusMode) {
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        m_FocusMode = false;
    }
    
}

