#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum class CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
	Jump,
	Down
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


class Camera
{

public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	// Euler angles
	float Yaw;
	float Pitch;

	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;


	Camera(glm::vec3 position = glm::vec3(0.0f, 2.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
	
	void Update(float deltaTime);

	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;
	glm::mat4 GetViewProjectionMatrix();

	void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);

	void ProcessMouseScroll(float yOffset);


private:
	void UpdateCameraVectors();
	void SubscribeToEvents();

private:
	float m_PerspectiveFOV = glm::radians(45.0f);
	float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;
	
	int m_Width = 800, m_Height = 600;

	bool m_MoveForward = false;
	bool m_MoveBackward = false;
	bool m_MoveLeft = false;
	bool m_MoveRight = false;
	bool m_MoveUp = false;
	bool m_MoveDown = false;

};