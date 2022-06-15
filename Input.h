#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"

float screenWidth = 1920.0f;
float screenHeight = 1080.0f;

float lastX = screenWidth / 2, lastY = screenHeight / 2;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;


void mouseCallback(GLFWwindow* window, double x, double y)
{
    if (firstMouse)
    {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    float xOffset = x - lastX;
    float yOffset = lastY - y;
    lastX = x;
    lastY = y;

    Camera::MAIN.processMouseMovement(xOffset, yOffset, deltaTime);
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    bool fast = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);
    bool slow = (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Camera::MAIN.processKeyboard(FORWARD, deltaTime, fast, slow);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Camera::MAIN.processKeyboard(BACKWARD, deltaTime, fast, slow);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Camera::MAIN.processKeyboard(RIGHT, deltaTime, fast, slow);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Camera::MAIN.processKeyboard(LEFT, deltaTime, fast, slow);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        Camera::MAIN.processKeyboard(UP, deltaTime, fast, slow);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        Camera::MAIN.processKeyboard(DOWN, deltaTime, fast, slow);
}


/**
* Resizes viewport when window is resized
*/

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    screenWidth = width;
    screenHeight = height;
}