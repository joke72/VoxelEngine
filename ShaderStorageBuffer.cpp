#include "ShaderStorageBuffer.h"
ShaderStorageBuffer::ShaderStorageBuffer(unsigned int size)
{
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, NULL, GL_DYNAMIC_COPY);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_RendererID);
}

ShaderStorageBuffer::ShaderStorageBuffer(const void* data, unsigned int size, GLenum usage)
{
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, usage);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_RendererID);
}

ShaderStorageBuffer::~ShaderStorageBuffer(){}

void ShaderStorageBuffer::BufferData(const void* data, unsigned int size, unsigned int offset) const
{
	Bind();
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data);
//	GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
//	memcpy(p, data, size);
//	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	Unbind();
}

void ShaderStorageBuffer::Bind() const
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
}

void ShaderStorageBuffer::Unbind() const
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

unsigned int ShaderStorageBuffer::GetRendererID() const
{
	return m_RendererID;
}