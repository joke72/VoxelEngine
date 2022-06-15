#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Constants.h"
#include "ChunkRenderer.h"
#include "ChunkRenderData.h"
#include "VertexArray.h"



class Chunk
{
private:

	static const GLuint MAX_VERTEX_COUNT = CHUNK_VOLUME * 24;


	void AddBlock(GLint blockPosition);
	void AddFace(GLuint faceIndex, GLuint blockData);
	GLuint GetBlock(int x, int y, int z);
public:
	ChunkRenderData m_RenderData;
	/*
	glm::vec3 m_Position;
	GLuint m_IndexCount;
	*/
	GLuint* m_Mesh;
	GLuint* m_Data;

	VertexBuffer m_VBO;

	GLuint m_VertexCount;

	GLuint* GenerateData();
	GLuint* GenerateMesh();
	void RecalculateMesh();

	Chunk(const glm::vec3& position, const ChunkRenderer& renderer);
	~Chunk();

};

