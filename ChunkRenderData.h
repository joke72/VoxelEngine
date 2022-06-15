#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "VertexArray.h"

struct ChunkRenderData
{
	glm::vec3 m_Position;
	VertexArray m_VAO;
	GLuint m_IndexCount;

	ChunkRenderData(glm::vec3 position) :
		m_Position(position),
		m_VAO(),
		m_IndexCount(0) {}
};
