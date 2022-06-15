#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Chunk.h"
#include "Shader.h"
#include "VertexArray.h"

class ChunkRenderer
{
private:

	GLuint* m_ChunkVertices;
	GLuint* m_ChunkIndices;
	VertexArray m_VAO;
	VertexBuffer m_VBO;
	IndexBuffer m_IBO;
	VertexBufferLayout m_Layout;
	GLuint m_IndexCount;
	GLuint m_VertexCount;

	GLuint* GenerateVertexArray();
	GLuint* GenerateIndexArray();
public:

	ChunkRenderer();
	~ChunkRenderer();

	void RecalculateMesh(const GLuint* chunkData);
	void Render(const Chunk& chunk, const Shader& shader);

	GLuint GetMaxIndexCount() const;
	GLuint GetMaxVertexCount() const;



};

