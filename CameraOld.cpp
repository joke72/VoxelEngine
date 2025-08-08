/*#include "CameraRTS.h"

CameraRTS CameraRTS::MAIN;

CameraRTS::CameraRTS() : m_Position(glm::vec3(0.0f, 32.0f, 0.0f)), m_Up(glm::vec3(0.0f, 1.0f, 0.0f)), m_Yaw(-90.0f), m_Pitch(0.0f), m_Sensitivity(0.1f), m_MovementSpeed(10.0f)
{
	updateCameraVectors();
}

CameraRTS::CameraRTS(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : m_Position(position), m_Up(up), m_Yaw(yaw), m_Pitch(pitch), m_Sensitivity(0.1f), m_MovementSpeed(10.0f)
{
	updateCameraVectors();
}

glm::mat4 CameraRTS::getViewMatrix()
{
	return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void CameraRTS::processMouseMovement(float xOffset, float yOffset, float deltaTime)
{
	xOffset *= m_Sensitivity;
	yOffset *= m_Sensitivity;
	rotate(xOffset, yOffset, deltaTime);
}

void CameraRTS::processKeyboard(CameraMovement direction, float deltaTime, bool fast, bool slow)
{
	float velocity = m_MovementSpeed * 2.0f;
	velocity += velocity * fast * 2.0f;
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

void CameraRTS::moveForward(float dz, float deltaTime)
{
	m_Position += m_Front * dz * deltaTime;
}
void CameraRTS::moveUp(float dy, float deltaTime)
{
	m_Position += m_Up * dy * deltaTime;
}
void CameraRTS::moveRight(float dx, float deltaTime)
{
	m_Position += m_Right * dx * deltaTime;
}

void CameraRTS::rotate(float y, float p, float deltaTime)
{
	m_Yaw += y * 1;
	m_Pitch += p * 1;
	if (m_Pitch > 89.0f)
		m_Pitch = 89.0f;
	if (m_Pitch < -89.0f)
		m_Pitch = -89.0f;

	updateCameraVectors();
}

void CameraRTS::updateCameraVectors()
{
	m_Front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front.y = sin(glm::radians(m_Pitch));
	m_Front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

	m_Front = glm::normalize(m_Front);
	m_Right = glm::normalize(glm::cross(m_Front, WORLDUP));
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));

}*/