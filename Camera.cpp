#include "Camera.h"

Camera Camera::MAIN;

Camera::Camera() : m_Position(glm::vec3(0.0f, 0.0f, 3.0f)), m_Up(glm::vec3(0.0f, 1.0f, 0.0f)), m_Yaw(-90.0f), m_Pitch(0.0f), m_Sensitivity(0.1f), m_MovementSpeed(10.0f)
{
	updateCameraVectors();
}

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : m_Position(position), m_Up(up), m_Yaw(yaw), m_Pitch(pitch), m_Sensitivity(0.1f), m_MovementSpeed(10.0f)
{
	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void Camera::processMouseMovement(float xOffset, float yOffset, float deltaTime)
{
	xOffset *= m_Sensitivity;
	yOffset *= m_Sensitivity;
	rotate(xOffset, yOffset, deltaTime);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime, bool fast, bool slow)
{
	float velocity = m_MovementSpeed;
	velocity += velocity * fast * 3;
	if (slow)
		velocity = velocity * 0.25f;

	if (direction == FORWARD)
		moveForward(velocity, deltaTime);
	if (direction == BACKWARD)
		moveForward(-velocity, deltaTime);
	if (direction == RIGHT)
		moveRight(velocity, deltaTime);
	if (direction == LEFT)
		moveRight(-velocity, deltaTime);
	if (direction == UP)
		moveUp(velocity, deltaTime);
	if (direction == DOWN)
		moveUp(-velocity, deltaTime);
}

void Camera::moveForward(float dz, float deltaTime)
{
	m_Position += m_Front * dz * deltaTime;
}
void Camera::moveUp(float dy, float deltaTime)
{
	m_Position += m_Up * dy * deltaTime;
}
void Camera::moveRight(float dx, float deltaTime)
{
	m_Position += m_Right * dx * deltaTime;
}

void Camera::rotate(float y, float p, float deltaTime)
{
	m_Yaw += y * 1;
	m_Pitch += p * 1;
	if (m_Pitch > 89.0f)
		m_Pitch = 89.0f;
	if (m_Pitch < -89.0f)
		m_Pitch = -89.0f;

	updateCameraVectors();
}

void Camera::updateCameraVectors()
{
	m_Front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front.y = sin(glm::radians(m_Pitch));
	m_Front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

	m_Front = glm::normalize(m_Front);
	m_Right = glm::normalize(glm::cross(m_Front, WORLDUP));
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));

}