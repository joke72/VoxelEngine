#pragma once
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;
};


class VertexBufferLayout
{

private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride = 0;

public:

	void AddVertexAttribute(unsigned int type, unsigned int count);

	const std::vector<VertexBufferElement>& GetElements() const;

	unsigned int GetStride() const;


	static unsigned int GetTypeSize(unsigned int type);

};

