#include "Camera.h"
#include "Events/EventDispatcher.h"
#include <GLFW/glfw3.h>

#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw , float pitch )
	: Position(position), Front(glm::vec3(0.0f, 0.0f, -1.0f)), WorldUp(up),
	Yaw(yaw), Pitch(pitch), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	UpdateCameraVectors();
	SubscribeToEvents();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	: Position(glm::vec3(posX, posY, posZ)), Front(glm::vec3(0.0f, 0.0f, -1.0f)),
	WorldUp(glm::vec3(upX, upY, upZ)), Yaw(yaw), Pitch(pitch), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	UpdateCameraVectors();
	SubscribeToEvents();
}

void Camera::SubscribeToEvents()
{
	g_EventDispatcher.AddEventListener<FramebufferResizeEvent>([this](const FramebufferResizeEvent& e) {
		m_Width = e.Width;
		m_Height = e.Height;
		});

	g_EventDispatcher.AddEventListener<KeyEvent>([this](const KeyEvent& e) {
		bool pressed = (e.Action != GLFW_RELEASE);
		switch (e.Key) {
		case GLFW_KEY_W:         m_MoveForward = pressed; break;
		case GLFW_KEY_S:         m_MoveBackward = pressed; break;
		case GLFW_KEY_A:         m_MoveLeft = pressed; break;
		case GLFW_KEY_D:         m_MoveRight = pressed; break;
		case GLFW_KEY_SPACE:     m_MoveUp = pressed; break;
		case GLFW_KEY_LEFT_CONTROL:m_MoveDown = pressed; break;
		}
		});

	g_EventDispatcher.AddEventListener<MouseMoveEvent>([this](const MouseMoveEvent& e) {
		ProcessMouseMovement(static_cast<float>(e.XOffset), static_cast<float>(e.YOffset));
		});

	g_EventDispatcher.AddEventListener<MouseScrollEvent>([this](const MouseScrollEvent& e) {
		ProcessMouseScroll(static_cast<float>(e.YOffset));
		});
}

void Camera::Update(float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (m_MoveForward)  Position += Front * velocity;
	if (m_MoveBackward) Position -= Front * velocity;
	if (m_MoveLeft)     Position -= Right * velocity;
	if (m_MoveRight)    Position += Right * velocity;
	if (m_MoveUp)       Position += WorldUp * velocity;
	if (m_MoveDown)     Position -= WorldUp * velocity;
}

glm::mat4 Camera::GetViewMatrix() const
{
	return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return glm::perspective(m_PerspectiveFOV, (float)m_Width / (float)m_Height, m_PerspectiveNear, m_PerspectiveFar);
}

glm::mat4 Camera::GetViewProjectionMatrix()
{
	return GetProjectionMatrix() * GetViewMatrix();
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch)
{
	xOffset *= MouseSensitivity;
	yOffset *= MouseSensitivity;

	Yaw += xOffset;
	Pitch += yOffset;
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}
	UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yOffset)
{
	Zoom -= yOffset;
	if (Zoom < 1.0f)
		Zoom = 1.0f;
	if (Zoom > 45.0f)
		Zoom = 45.0f;
}

void Camera::UpdateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}
