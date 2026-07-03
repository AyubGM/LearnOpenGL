#include "Window.h"
#include <GLFW/glfw3.h>
#include <print>

namespace Utils
{
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));

        if (win)
            win->OnFramebufferResize(width, height);
    }

    static void cursor_position_callback(GLFWwindow* window, double xposIn, double yposIn)
    {
        // Retrieve the Window instance from the user pointer
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (win)
            win->OnCursorPos(xposIn, yposIn);
    }

    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
        if (win)
            win->OnScroll(yoffset);
    }

}

Window::Window(uint32_t width, uint32_t height, std::string_view name, const Camera& camera) : 
    m_Width(width), m_Height(height), m_Name(name), m_Camera(camera), m_FirstMove(true)
{
    Init();
}

bool Window::Init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(m_Width, m_Height, m_Name.c_str(), NULL, NULL);

    if (m_Window == NULL)
    {
        std::println("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(m_Window);

    glfwSetWindowUserPointer(m_Window, this);

    glfwSetFramebufferSizeCallback(m_Window, Utils::framebuffer_size_callback);
    glfwSetCursorPosCallback(m_Window, Utils::cursor_position_callback);
    glfwSetScrollCallback(m_Window, Utils::scroll_callback);
    // tell GLFW to capture our mouse
    //glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    m_Camera.ProcessMouseMovement(static_cast<float>(xoffset), static_cast<float>(yoffset));
}

void Window::OnScroll(double yoffset)
{
    m_Camera.ProcessMouseScroll(static_cast<float>(yoffset));
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

    if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
        m_Camera.ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
    if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
        m_Camera.ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
    if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
        m_Camera.ProcessKeyboard(CameraMovement::LEFT, deltaTime);
    if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
        m_Camera.ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
    if (glfwGetKey(m_Window, GLFW_KEY_SPACE) == GLFW_PRESS)
        m_Camera.ProcessKeyboard(CameraMovement::Jump, deltaTime);
    if (glfwGetKey(m_Window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        m_Camera.ProcessKeyboard(CameraMovement::Down, deltaTime);
}

