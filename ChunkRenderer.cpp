#include "ChunkRenderer.h"


ChunkRenderer::ChunkRenderer() :
	m_ChunkIndices(GenerateIndexArray()),
	m_IBO(m_ChunkIndices, MAX_INDEX_COUNT * sizeof(GLuint))
{
}

ChunkRenderer::~ChunkRenderer()
{
	delete[] m_ChunkIndices;
}

GLuint* ChunkRenderer::GenerateIndexArray()
{
	GLuint* indices = new GLuint[MAX_INDEX_COUNT];

    int offset = 0;
	for (int i = 0; i < MAX_INDEX_COUNT; i+=6)
	{        
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 2 + offset;

		indices[i + 3] = 2 + offset;
		indices[i + 4] = 3 + offset;
		indices[i + 5] = 0 + offset;
		
        offset += 4;
        
        //std::cout << indices[i] << std::endl;
	}
    std::cout << "indices generated" << std::endl;
	return indices;
}


void ChunkRenderer::Render(const ChunkRenderData& chunkData, const Shader& shader) const
{
	shader.use();
    shader.setVec3("uChunkPosition", chunkData.m_Position);

	chunkData.m_VAO.Bind();
	glDrawElements(GL_TRIANGLES, chunkData.m_IndexCount, GL_UNSIGNED_INT, 0);
}

