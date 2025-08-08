#include "Camera.h"

Camera Camera::MAIN;

Camera::Camera() : m_Position(glm::vec3(0.0f, 32.0f, 0.0f)), m_Up(glm::vec3(0.0f, 1.0f, 0.0f)), m_Yaw(-90.0f), m_Pitch(0.0f), m_Sensitivity(0.1f), m_MovementSpeed(50.0f), m_LookAtPoint(m_Position)
{
	updateCameraVectors();
}

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : m_Position(position), m_Up(up), m_Yaw(yaw), m_Pitch(pitch), m_Sensitivity(0.1f), m_MovementSpeed(50.0f), m_LookAtPoint(m_Position)
{
	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(m_Position, m_Position + m_LookAtPoint, m_Up);
}

void Camera::processMouseMovement(float xOffset, float yOffset, float deltaTime)
{
	if (!right && !middle) return;
	xOffset *= m_Sensitivity;
	yOffset *= m_Sensitivity;
	rotate(xOffset, yOffset, deltaTime);
}

void Camera::Zoom(float dy, float deltaTime)
{
	m_Zoom -= dy * deltaTime * 500.0f;
	if (m_Zoom > 500.0f)
		m_Zoom = 500.0f;
	if (m_Zoom < 50.0f)
		m_Zoom = 50.0f;
	updateCameraVectors();
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime, bool fast, bool slow)
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
	updateCameraVectors();
}

void Camera::moveForward(float dz, float deltaTime)
{
	//m_Position	  += glm::normalize(glm::vec3(m_Front.x, 0, m_Front.z)) * dz * deltaTime;
	m_LookAtPoint += glm::normalize(glm::vec3(m_Front.x, 0, m_Front.z)) * dz * deltaTime;
}
void Camera::moveUp(float dy, float deltaTime)
{
	//m_Position	  += glm::vec3(0, 1, 0) * dy * deltaTime;
	if(m_LookAtPoint.y + dy * deltaTime >= 0 && m_LookAtPoint.y + dy * deltaTime <= CHUNK_HEIGHT / 2)
		m_LookAtPoint += glm::vec3(0, 1, 0) * dy * deltaTime;
}
void Camera::moveRight(float dx, float deltaTime)
{
	//m_Position	  += glm::normalize(glm::vec3(m_Right.x, 0, m_Right.z)) * dx * deltaTime;
	m_LookAtPoint += glm::normalize(glm::vec3(m_Right.x, 0, m_Right.z)) * dx * deltaTime;
}

void Camera::rotate(float y, float p, float deltaTime)
{/*
	if(right)
		m_Position -= glm::normalize(glm::vec3(m_Right.x, 0, m_Right.z)) * y * deltaTime * 50.0f;
	if(middle)
		m_Position += glm::normalize(m_Up) * p * deltaTime * 50.0f; */
	if(right || middle)
		m_Yaw += y * 1;
	if(right || middle)
		m_Pitch -= p * 1;
	if (m_Pitch > 89.0f)
		m_Pitch = 89.0f;
	if (m_Pitch < 10.0f)
		m_Pitch = 10.0f;

	updateCameraVectors();
}

void Camera::updateCameraVectors()
{

	//m_Front = m_LookAtPoint - m_Position;
	//m_Position = m_LookAtPoint - glm::vec3(cos(glm::radians(m_Yaw)), 0.0f, sin(glm::radians(m_Yaw)));
	m_Position = m_LookAtPoint + m_Zoom * glm::vec3(cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)), sin(glm::radians(m_Pitch)), sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch)));
	/*
	m_Front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front.y = sin(glm::radians(m_Pitch));
	m_Front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	*/
	m_Front = glm::normalize(m_LookAtPoint - m_Position);
	m_Right = glm::normalize(glm::cross(m_Front, WORLDUP));
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));

}