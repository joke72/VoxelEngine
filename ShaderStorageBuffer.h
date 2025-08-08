#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

class ShaderStorageBuffer
{
private:
	GLuint m_RendererID;

public:

	ShaderStorageBuffer(const void* data, unsigned int size, GLenum usage);
	ShaderStorageBuffer(unsigned int size);

	~ShaderStorageBuffer();

	void BufferData(const void* data, unsigned int size, unsigned int offset = 0) const;

	void Bind() const;

	void Unbind() const;

	GLuint GetRendererID() const;

};

