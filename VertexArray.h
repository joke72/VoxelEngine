#pragma once
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:

	unsigned int m_RendererID;

public:

	VertexArray();

	~VertexArray();

	void AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout);
	void AddBuffer(const VertexBuffer& vbo, const IndexBuffer& ibo, const VertexBufferLayout& layout);


	void Bind() const;

	void Unbind() const;

	unsigned int GetRendererID() const;

};
