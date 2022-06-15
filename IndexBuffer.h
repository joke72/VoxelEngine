#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class IndexBuffer
{
private:
	unsigned int m_RendererID;

public:

	IndexBuffer(const void* data, unsigned int size);

	~IndexBuffer();

	void Bind() const;

	static void Unbind();

	void BufferData(const void* data, unsigned int size) const;
};
