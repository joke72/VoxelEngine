#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "ChunkRenderData.h"
#include "Constants.h"
#include "Shader.h"
#include "VertexArray.h"


class ChunkRenderer
{
public:

	GLuint* m_ChunkIndices;
	IndexBuffer m_IBO;
	VertexBufferLayout m_Layout;


	void Render(const ChunkRenderData& chunk, const Shader& shader);

	ChunkRenderer();
	~ChunkRenderer();



private:
	static const GLuint MAX_INDEX_COUNT = CHUNK_VOLUME * 36;

	GLuint* GenerateIndexArray();
};

