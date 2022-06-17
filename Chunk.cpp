#include "Chunk.h"

Chunk::Chunk(const glm::vec3& position, const ChunkRenderer& renderer) : 
    m_RenderData(position),
	m_Mesh(GenerateMesh()),
    m_Data(GenerateData()),
	m_VBO(m_Mesh, MAX_VERTEX_COUNT * sizeof(GLuint), GL_DYNAMIC_DRAW)
{
    m_RenderData.m_VAO.AddBuffer(m_VBO, (renderer.m_IBO), VertexBufferLayout::blockLayout);

    RecalculateMesh();

}

GLuint* Chunk::GenerateData()
{
	GLuint* data = new GLuint[CHUNK_VOLUME];

	for (int i = 0; i < CHUNK_VOLUME; i++)
	{
        int r = rand();
        if (r > 20000)
            data[i] = 2;
        else if(r > 10000)
    		data[i] = 1;
        else
            data[i] = 0;
	}

	return data;
}


GLuint* Chunk::GenerateMesh()
{
	GLuint* vertices = new GLuint[MAX_VERTEX_COUNT];

	for (int i = 0; i < MAX_VERTEX_COUNT; i++)
	{
		vertices[i] = 0;
	}
	return vertices;
}

void Chunk::AddFace(GLuint faceIndex, GLuint blockData)
{
    for (int i = 0; i < 4; i++)
    {
        m_Mesh[m_VertexCount + i] = BLOCK_MESH[i + faceIndex * 4] + blockData;
    }
    m_VertexCount += 4;
    m_RenderData.m_IndexCount += 6;

}

GLuint Chunk::GetBlock(int x, int y, int z)
{
    if (x < 0 || y < 0 || z < 0 || x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE)
        return 0;
    return m_Data[x + y * CHUNK_SIZE + z * CHUNK_AREA];
}


void Chunk::AddBlock(GLint blockPosition)
{
    GLint offsets[6] = { -CHUNK_SIZE, CHUNK_SIZE, -1, 1, -CHUNK_AREA, CHUNK_AREA};

    int xPos = blockPosition % CHUNK_SIZE;
    int yPos = (blockPosition / CHUNK_SIZE) % CHUNK_SIZE;
    int zPos = blockPosition / (CHUNK_AREA);

    GLuint blockData = (m_Data[blockPosition] << 24) + (xPos << 12) + (yPos << 6) + zPos;

    if (GetBlock(xPos, yPos, zPos - 1) == 0)
        AddFace(0, blockData);
    if (GetBlock(xPos, yPos, zPos + 1) == 0)
        AddFace(1, blockData);

    if (GetBlock(xPos - 1, yPos, zPos) == 0)
        AddFace(2, blockData);
    if (GetBlock(xPos + 1, yPos, zPos) == 0)
        AddFace(3, blockData);

    if (GetBlock(xPos, yPos - 1, zPos) == 0)
        AddFace(4, blockData);
    if (GetBlock(xPos, yPos + 1, zPos) == 0)
        AddFace(5, blockData);
    
}

void Chunk::RecalculateMesh()
{

    m_VertexCount = 0;
    m_RenderData.m_IndexCount = 0;
    int offset = 0;
    for (int i = 0; i < CHUNK_VOLUME; i++)
    {
        int xPos = i % CHUNK_SIZE;
        int yPos = i / (CHUNK_SIZE * CHUNK_SIZE);
        int zPos = (i / CHUNK_SIZE) % CHUNK_SIZE;

        if (m_Data[i] != 0)
        {
            AddBlock(i);
        }
    }

    m_VBO.BufferData(m_Mesh, m_VertexCount * sizeof(GLuint));

}

Chunk::~Chunk()
{
	delete[] m_Data;
	delete[] m_Mesh;
	//std::cout << "chunk destroyed" << std::endl;
}