#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera
{

private:

	glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 m_Right = glm::vec3(1.0f, 0.0f, 0.0f);

	const glm::vec3 WORLDUP = glm::vec3(0.0f, 1.0f, 0.0f);

	void updateCameraVectors();

	float m_Yaw, m_Pitch;
	float m_Sensitivity;
	float m_MovementSpeed;

public:

	static Camera MAIN;

	glm::vec3 m_Position;

	Camera();

	Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);

	glm::mat4 getViewMatrix();

	void processMouseMovement(float xOffset, float yOffset, float deltaTime);

	void processKeyboard(CameraMovement direction, float deltaTime, bool fast, bool slow);

	void moveForward(float dz, float deltaTime);
	void moveUp(float dy, float deltaTime);
	void moveRight(float dx, float deltaTime);

	void rotate(float y, float p, float deltaTime);
};

