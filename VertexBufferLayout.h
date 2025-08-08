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
	bool instance;
};


class VertexBufferLayout
{

private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride = 0;

public:

	static VertexBufferLayout blockLayout;
	static VertexBufferLayout defaultLayout;
	static VertexBufferLayout screenLayout;

	static void Init();
	void AddVertexAttribute(unsigned int type, unsigned int count, bool instance = false);

	const std::vector<VertexBufferElement>& GetElements() const;

	unsigned int GetStride() const;

	static unsigned int GetTypeSize(unsigned int type);


};

