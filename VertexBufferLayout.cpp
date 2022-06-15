#pragma once
#include "VertexBufferLayout.h"



void VertexBufferLayout::AddVertexAttribute(unsigned int type, unsigned int count)
{
	m_Elements.push_back({ type, count, GL_FALSE });
	m_Stride += GetTypeSize(type) * count;
}

const std::vector<VertexBufferElement>& VertexBufferLayout::GetElements() const
{
	return m_Elements;
}

unsigned int VertexBufferLayout::GetStride() const
{
	return m_Stride;
}


unsigned int VertexBufferLayout::GetTypeSize(unsigned int type)
{
	switch (type)
	{
	case GL_FLOAT:
		return sizeof(GLfloat);
	case GL_UNSIGNED_INT:
		return sizeof(GLuint);
	case GL_UNSIGNED_BYTE:
		return sizeof(GLubyte);
	case GL_UNSIGNED_SHORT:
		return sizeof(GLushort);
	}

	return -1;
}


