#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Constants.h"


class Chunk
{
public:
	uint32_t* m_ChunkData;
	glm::ivec3 m_ChunkPosition;
	bool rendered = false;
	
	Chunk(glm::ivec3 pos = glm::ivec3(0, 0, 0)) : m_ChunkData(new uint32_t[CHUNK_VOLUME]), m_ChunkPosition(pos)
	{
		/*
		for (int x = 0; x < CHUNK_SIZE; x++)
		{
			for (int y = 0; y < CHUNK_HEIGHT; y++)
			{
				for (int z = 0; z < CHUNK_SIZE; z++)
				{
					int r = rand();
					if (y < 4 * sin(x * 0.2) + 4 * cos(z * 0.2) + m_ChunkPosition.x)
						m_ChunkData[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_HEIGHT] = r < 15000 ? 1 : 2;
					else
						m_ChunkData[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_HEIGHT] = 0;
				}
			}
		}*/

		for (int i = 0; i < CHUNK_VOLUME; i++)
			m_ChunkData[i] = 0;
		/*
		for (int x = 0; x < CHUNK_SIZE; x++)
		{
			for (int y = 0; y < CHUNK_HEIGHT; y++)
			{
				for (int z = 0; z < CHUNK_SIZE; z++)
				{
					int r = rand();
					if (y < db::perlin(0.01 * (x + m_ChunkPosition.x * CHUNK_SIZE), 0.01 * (z + m_ChunkPosition.z * CHUNK_SIZE))*50 + 10)
						m_ChunkData[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_HEIGHT] = r < 15000 ? 1 : 2;
				}
			}
		}*/


		//std::cout << "Chunk created" << std::endl;
	}

	~Chunk()
	{
		delete[] m_ChunkData;
		//std::cout << "Chunk destroyed" << std::endl;
	}

};

