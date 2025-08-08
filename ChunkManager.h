#pragma once
#include <vector>
#include "Chunk.h"
#include "ShaderStorageBuffer.h"
#define DB_PERLIN_IMPL
#include "db_perlin.hpp"

void test()
{

}

static uint8_t m_ChunkData[ARRAY_SIZE];
class ChunkManager
{

	ShaderStorageBuffer m_SSB;
	bool* m_ShouldUpdate;


public:
	glm::ivec3* m_LoadedChunk;
	bool working = true;
	bool adding = false;
	int first = CHUNKS, last = 0;
	const int m_Octaves = 3;
	glm::vec2 m_OctaveOffsets[3] = { {},{},{} };


	ChunkManager() : m_SSB(ARRAY_SIZE * sizeof(GLuint)), m_LoadedChunk(new glm::ivec3[CHUNKS]), m_ShouldUpdate(new bool[CHUNKS])
	{

		for (int i = 0; i < ARRAY_SIZE; i++)
		{
			if (i < CHUNKS)
				m_ChunkData[i] = i;
			else
				m_ChunkData[i] = 0;
		}
		std::cout << "init done" << std::endl;
		m_SSB.BufferData(m_ChunkData, CHUNKS * sizeof(uint8_t), 0);

		std::cout << "buffer data done" << std::endl;
		for (int x = 0; x < CHUNKS_X; x++)
		{

			for (int z = 0; z < CHUNKS_Z; z++)
			{
				m_LoadedChunk[x + z * CHUNKS_X] = glm::ivec3(x + CHUNKS_X*2, 0, z + CHUNKS_Z*2);
				//LoadChunk(x, z);
			}
		}
		std::cout << "loaded chunks done" << std::endl;

		srand(20984);

		for (int i = 0; i < m_Octaves; i++) {
			float offsetX = (float)(rand()) / (float)(RAND_MAX) * 200000 - 100000;
			float offsetY = (float)(rand()) / (float)(RAND_MAX) * 200000 - 100000;
			m_OctaveOffsets[i] = glm::vec2(offsetX, offsetY);
		}
		int a;
		Up(a, false);
		Up(a, false);
		Up(a, false);
		
		for (int i = 0; i < CHUNKS; i++)
			m_ShouldUpdate[i] = true;
		std::cout << "should update done" << std::endl;
		first = 0;
		last = CHUNKS;

		BufferChunks();

		std::cout << "octaves done" << std::endl;
	}

	~ChunkManager()
	{
		working = false;
		delete[] m_LoadedChunk;
//		delete[] m_ChunkData;
		delete[] m_ShouldUpdate;
	}

	float dist(glm::ivec3 a, glm::ivec3 b)
	{
		float distX = b.x - a.x;
		float distY = b.y - a.y;
		float distZ = b.z - a.z;
		return sqrt(distX * distX + distY * distY + distZ * distZ);
	}


	glm::ivec3 GetClosest(int x, int z)
	{
		glm::vec3 cameraPos = Camera::MAIN.m_Position;
		glm::ivec3 cameraRenderPos = glm::ivec3((cameraPos.x / CHUNK_SIZE) / CHUNKS_X, 0, (cameraPos.z / CHUNK_SIZE) / CHUNKS_Z);
		glm::ivec3 vec(x, 0, z);
		
		float smallestDist = 9999.0f;
		glm::ivec3 result = cameraRenderPos;

		glm::ivec3 checkPos = cameraRenderPos + vec;

		for (int x = -1; x < 2; x++)
		{
			for (int y = -1; y < 2; y++)
			{
				glm::ivec3 posCheck = cameraRenderPos + glm::ivec3(x * CHUNKS_X, 0, y * CHUNKS_Z);
				float d = dist(posCheck, vec);
				if (d < smallestDist)
				{
					smallestDist = d;
					result = posCheck;
				}

			}
		}

		return result;

	}

	void join()
	{
	}

	void UpdateChunk()
	{

		using namespace std::literals::chrono_literals;

		while (working)
		{
			int chunksCreated = 0;
			adding = true;
			double currentTime = glfwGetTime();
			Up(chunksCreated, false);
			/*
			for (int i = 0; i < m_ChunksToUpdate.size(); i++)
			{
				if (m_ChunksToUpdate[i].rendered)
					m_ChunksToUpdate.erase(m_ChunksToUpdate.begin() + i);
			}*/
			adding = false;
			//std::cout << "Updated chunk in: " << (glfwGetTime() - currentTime) << "s" << std::endl;
			std::this_thread::sleep_for(0.1s);
		}
	}

	void Up(int& chunksCreated, bool firstTime = false)
	{
		for (int x = 0; x < CHUNKS_X; x++)
		{
			for (int z = 0; z < CHUNKS_Z; z++)
			{
				glm::ivec3 currentlyLoaded = m_LoadedChunk[x + z * CHUNKS_Y * CHUNKS_Z];
				glm::ivec3 cameraIntPos = glm::ivec3((int)Camera::MAIN.m_Position.x, 0, (int)Camera::MAIN.m_Position.z);
				glm::ivec3 cameraChunkPos = glm::ivec3((cameraIntPos.x / CHUNK_SIZE), 0, (cameraIntPos.z / CHUNK_SIZE));


				int xDist = cameraIntPos.x - currentlyLoaded.x * CHUNK_SIZE;
				int zDist = cameraIntPos.z - currentlyLoaded.z * CHUNK_SIZE;


				int correctX = currentlyLoaded.x;
				int correctZ = currentlyLoaded.z;


				if (abs(xDist) > abs(cameraIntPos.x - (currentlyLoaded.x + CHUNKS_X) * CHUNK_SIZE))
				{
					correctX += CHUNKS_X;
				}
				else if (abs(xDist) > abs(cameraIntPos.x - (currentlyLoaded.x - CHUNKS_X) * CHUNK_SIZE))
				{
					correctX -= CHUNKS_X;
				}
				if (abs(zDist) > abs(cameraIntPos.z - (currentlyLoaded.z + CHUNKS_Z) * CHUNK_SIZE))
				{
					correctZ += CHUNKS_Z;
				}
				else if (abs(zDist) > abs(cameraIntPos.z - (currentlyLoaded.z - CHUNKS_Z) * CHUNK_SIZE))
				{
					correctZ -= CHUNKS_Z;
				}



				if (correctX != currentlyLoaded.x || correctZ != currentlyLoaded.z || firstTime)
				{
					LoadChunk(correctX, correctZ);


					chunksCreated++;
					/*
					//m_ChunksToUpdate.insert(m_ChunksToUpdate.begin(), Chunk(glm::ivec3(correctX, 0, correctZ)));
					if (m_ChunksToUpdate.size() >= 40)
						continue;
					m_ChunksToUpdate.emplace_back(glm::ivec3(correctX, 0, correctZ));
					std::cout << "Adding" << m_ChunksToUpdate.size() << std::endl;
					//LoadChunk(correctX, correctZ);
					*/
				}
			}
		}
	}


	float getNoiseHeight(float x, float y, float scale, float persistance, float lacunarity)
	{

		if (scale <= 0.0f)
			scale = 0.001f;

		float amplitude = 1.0f;
		float frequency = 1.0f;
		float noiseHeight = 0.0f;

		for (int i = 0; i < m_Octaves; i++) {
			float X = x / scale * frequency + (m_OctaveOffsets[i].x);
			float Y = y / scale * frequency + (m_OctaveOffsets[i].y);

			float perlinValue = (float) db::perlin(X, Y, 0.0f);
			noiseHeight += perlinValue * amplitude;

			amplitude *= persistance;
			frequency *= lacunarity;
		}
		return noiseHeight;
	}

	int GetVoxelFromHeight(float height)
	{
		if (height < -0.2f)
		{
			return 3;
		}
		else if (height < -0.1f)
		{
			return 3;
		}
		else if (height < -0.05f)
		{
			return 4;
		}
		else if (height < 0.10f)
		{
			return 1;
		}
		else if (height < 0.4f)
		{
			return 17;
		}
		else if (height < 0.6f)
		{
			return 2;
		}
		else
		{
			return 6;
		}
	}

	void GenerateTree(int x, int z, int chunkID, int h, int treeHeight)
	{
			int xOffset = 0;
			int zOffset = 0;
			int pos = x + xOffset + (z + zOffset) * CHUNK_SIZE * CHUNK_HEIGHT + (CHUNKS + chunkID * CHUNK_VOLUME);

			for (int y = h; y < h + treeHeight; y++)
			{
				if (pos + y * CHUNK_SIZE >= 0 && pos + y * CHUNK_SIZE < ARRAY_SIZE)
				{
					m_ChunkData[pos + y * CHUNK_SIZE] = 5;
				}

			}
			pos = pos + (int)(h + treeHeight) * CHUNK_SIZE;
			if (pos >= 0 && pos < ARRAY_SIZE)
			{
				m_ChunkData[pos] = 17;
			}
			pos++;
			pos -= CHUNK_SIZE;
			if (pos >= 0 && pos < ARRAY_SIZE)
			{
				m_ChunkData[pos] = 17;
			}
			pos -= 2;
			if (pos >= 0 && pos < ARRAY_SIZE)
			{
				m_ChunkData[pos] = 17;
			}
			pos += 1 + CHUNK_SIZE * CHUNK_HEIGHT;
			if (pos >= 0 && pos < ARRAY_SIZE)
			{
				m_ChunkData[pos] = 17;
			}
			pos -= 2 * CHUNK_SIZE * CHUNK_HEIGHT;
			if (pos >= 0 && pos < ARRAY_SIZE)
			{
				m_ChunkData[pos] = 17;
			}

	}

	void LoadChunk(int chunkPosX, int chunkPosZ)
	{

		int chunkX = chunkPosX % CHUNKS_X;
		int chunkZ = chunkPosZ % CHUNKS_Z;
		if (chunkX < 0)
			chunkX = CHUNKS_X + chunkX;
		if (chunkZ < 0)
			chunkZ = CHUNKS_Z + chunkZ;


		int chunkID = (chunkX)
			+(0) * CHUNKS_X
			+ (chunkZ)*CHUNKS_Y * CHUNKS_Z;
		m_LoadedChunk[chunkID] = glm::ivec3(chunkPosX, 0, chunkPosZ);

		if (first > chunkID)
			first = chunkID;
		if (last < chunkID + 1)
			last = chunkID + 1;
		float maxHeight = -2000.0f;
		float minHeight = 2000.0f;

		for (int i = 0; i < CHUNK_VOLUME; i++)
		{
			m_ChunkData[i + (CHUNKS + chunkID * CHUNK_VOLUME)] = 0;
		}

		for (int z = 0; z < CHUNK_SIZE; z++)
		{
			//for (int y = 0; y < CHUNK_HEIGHT; y++)
			{
				for (int x = 0; x < CHUNK_SIZE; x++)
				{
					int r = rand();
					float height = getNoiseHeight((x + chunkPosX * CHUNK_SIZE) * 1.0f, (z + chunkPosZ * CHUNK_SIZE) * 1.0f, 200.0f, 0.36f, 4.28f);
					if (height > maxHeight)
						maxHeight = height;
					if (height < minHeight)
						minHeight = height;
					int terrainOffset = 20;
					int terrainAmplitude = 80;
					int blockHeight = height * terrainAmplitude + terrainOffset;
					
					for (int y = 0; y < CHUNK_HEIGHT; y++)
					{

						if (y == 0)
						{
							m_ChunkData[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_HEIGHT + (CHUNKS + chunkID * CHUNK_VOLUME)] = 4;
						}
						else
						{
							if (y < blockHeight || y < -0.1f * terrainAmplitude + terrainOffset)//db::perlin(0.01 * (x + chunkPosX * CHUNK_SIZE), 0.01 * (z + chunkPosZ * CHUNK_SIZE)) * 50 + 20
								m_ChunkData[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_HEIGHT + (CHUNKS + chunkID * CHUNK_VOLUME)] = GetVoxelFromHeight(height);
							else
							{
								//m_ChunkData[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_HEIGHT + (CHUNKS + chunkID * CHUNK_VOLUME)] = 0;
							}
						}


					}

					if (rand() < 500 && GetVoxelFromHeight(height) == 17)
					{
						int treeHeight = ((float)rand() / (float)RAND_MAX) * 8 + 6;
						GenerateTree(x, z, chunkID, blockHeight, treeHeight);
					}
				}
			}
		}
		m_ShouldUpdate[chunkID] = true;
		//m_SSB.BufferData(chunk.m_ChunkData, CHUNK_VOLUME * sizeof(GLuint), (CHUNKS + chunkID * CHUNK_VOLUME) * sizeof(GLuint));
		//std::cout << "Max Height: " << maxHeight << std::endl;
		//std::cout << "Min Height: " << minHeight << std::endl;

	}

	void BufferChunks()
	{
		if (adding)
			return;
		
		m_SSB.BufferData(m_ChunkData, CHUNKS * sizeof(uint8_t), 0);
		for (int i = 0; i < CHUNKS; i++)
		{
			/*
			int j;
			for(j = 0; i + j < CHUNKS; j++)
			{
				if (m_ShouldUpdate[i + j] == false)
					break;

				m_ShouldUpdate[i + j] = false;
			}
			std::cout << "j: " << j << std::endl;
			m_SSB.BufferData(m_ChunkData + i * CHUNK_VOLUME + CHUNKS, j * CHUNK_VOLUME * sizeof(uint32_t), (i * CHUNK_VOLUME + CHUNKS) * sizeof(uint32_t));
			if(j > 0)
				i += j - 1;
			*/
			
			if (m_ShouldUpdate[i])
			{
				m_SSB.BufferData(m_ChunkData + i * CHUNK_VOLUME + CHUNKS, CHUNK_VOLUME * sizeof(uint8_t), (i * CHUNK_VOLUME + CHUNKS) * sizeof(uint8_t));
				m_ShouldUpdate[i] = false;

			}
		}
		first = CHUNKS;
		last = 0;
		/*
		for(int i = 0; i < m_ChunksToUpdate.size(); i++)
		{
				
			Chunk* currentChunk = &m_ChunksToUpdate[i];
			if (currentChunk->rendered || adding)
				continue;
				
			int chunkX = currentChunk->m_ChunkPosition.x % CHUNKS_X;
			int chunkZ = currentChunk->m_ChunkPosition.z % CHUNKS_Z;
			if (chunkX < 0)
				chunkX = CHUNKS_X + chunkX;
			if (chunkZ < 0)
				chunkZ = CHUNKS_Z + chunkZ;


			int chunkID = (chunkX)
				+(0) * CHUNKS_X
				+ (chunkZ)*CHUNKS_Y * CHUNKS_Z;
			m_LoadedChunk[chunkID] = glm::ivec3(currentChunk->m_ChunkPosition.x, 0, currentChunk->m_ChunkPosition.z);

			m_SSB.BufferData(currentChunk->m_ChunkData, CHUNK_VOLUME * sizeof(GLuint), (CHUNKS + chunkID * CHUNK_VOLUME) * sizeof(GLuint));
			currentChunk->rendered = true;
		}
		*/

	}

	void LoadChunk(const Chunk& chunk)
	{
		m_SSB.BufferData(chunk.m_ChunkData, CHUNK_VOLUME * sizeof(uint8_t), CHUNKS * sizeof(uint8_t));
	}

};
