#pragma once
#include "VertexArray.h"


VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RendererID);
	Bind();
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}


void VertexArray::AddBuffer(const VertexBuffer& vbo, const IndexBuffer& ibo, const VertexBufferLayout& layout)
{
	Bind();
	vbo.Bind();
	ibo.Bind();

	std::vector<VertexBufferElement> elements = layout.GetElements();

	int offset = 0;
	for (int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (void*)offset);
		offset += element.count * VertexBufferLayout::GetTypeSize(element.type);
	}
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind()
{
	glBindVertexArray(0);
}

unsigned int VertexArray::GetRendererID() const
{
	return m_RendererID;
}
