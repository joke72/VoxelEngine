#pragma once
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const void* data, unsigned int size)
{

	std::cout << "index buffer initialized" << std::endl;

	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::BufferData(const void* data, unsigned int size) const
{
	Bind();
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
}
void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
