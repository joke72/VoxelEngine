#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

const int CHUNK_SIZE = 15;
const int CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

class Chunk
{

public:
	glm::vec3 m_Position;
	GLuint* m_ChunkData;

	Chunk(const glm::vec3& position);
	~Chunk();

};

