#include "Chunk.h"

Chunk::Chunk(const glm::vec3& position) : 
	m_Position(position),
	m_ChunkData(new GLuint[CHUNK_VOLUME])
{
	for (int i = 0; i < CHUNK_VOLUME; i++)
	{
		m_ChunkData[i] = i % 2;
	}
	
	
	std::cout << "chunk created" << std::endl;


}

Chunk::~Chunk()
{
	delete [] m_ChunkData;
	std::cout << "chunk destroyed" << std::endl;
}