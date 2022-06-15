#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


class VertexBuffer
{
private:
	unsigned int m_RendererID;

public:

	VertexBuffer(const void* data, unsigned int size, GLenum usage);

	~VertexBuffer();

	void BufferData(const void* data, unsigned int size) const;

	void Bind() const;
	
	static void Unbind();
};
