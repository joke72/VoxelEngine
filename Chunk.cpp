#include "Chunk.h"

Chunk::Chunk(const glm::vec3& position, const ChunkRenderer& renderer) : 
    m_RenderData(position),
	m_Mesh(GenerateMesh()),
    m_Data(GenerateData()),
	m_VBO(m_Mesh, MAX_VERTEX_COUNT * sizeof(GLuint), GL_DYNAMIC_DRAW)
{
    m_RenderData.m_VAO.AddBuffer(m_VBO, (renderer.m_IBO), renderer.m_Layout);

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
    GLuint cube[] = {
        // 8 bits : ID, 2 bits unused, 4 bits : normals, 18 bits : chunk_position  
        // FAR
        //0b1001
        0b00000000001001000000000000000000,
        0b00000000011001000000000001000000,
        0b00000000111001000001000001000000,
        0b00000000101001000001000000000000,

        // NEAR
        //0b0001
        0b00000000000001000000000000000001,
        0b00000000010001000001000000000001,
        0b00000000110001000001000001000001,
        0b00000000100001000000000001000001,

        // LEFT
        //0b1100
        0b00000000001100000000000001000001,
        0b00000000011100000000000001000000,
        0b00000000111100000000000000000000,
        0b00000000101100000000000000000001,

        // RIGHT
        //0b0100
        0b00000000000100000001000001000000,
        0b00000000010100000001000001000001,
        0b00000000110100000001000000000001,
        0b00000000100100000001000000000000,

        // BOTTOM
        //0b1010
        0b00000000001010000000000000000000,
        0b00000000011010000001000000000000,
        0b00000000111010000001000000000001,
        0b00000000101010000000000000000001,

        //TOP
        //0b0010
        0b00000000000010000000000001000000,
        0b00000000010010000000000001000001,
        0b00000000110010000001000001000001,
        0b00000000100010000001000001000000
    };

    for (int i = 0; i < 4; i++)
    {
        m_Mesh[m_VertexCount + i] = cube[i + faceIndex * 4] + blockData;
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
    GLuint cube[] = {
        // 8 bits : ID, 2 bits unused, 4 bits : normals, 18 bits : chunk_position  
        // FAR
        //0b1001
        0b00000000001001000000000000000000,
        0b00000000011001000000000001000000,
        0b00000000111001000001000001000000,
        0b00000000101001000001000000000000,

        // NEAR
        //0b0001
        0b00000000000001000000000000000001,
        0b00000000010001000001000000000001,
        0b00000000110001000001000001000001,
        0b00000000100001000000000001000001,

        // LEFT
        //0b1100
        0b00000000001100000000000001000001,
        0b00000000011100000000000001000000,
        0b00000000111100000000000000000000,
        0b00000000101100000000000000000001,

        // RIGHT
        //0b0100
        0b00000000000100000001000001000000,
        0b00000000010100000001000001000001,
        0b00000000110100000001000000000001,
        0b00000000100100000001000000000000,

        // BOTTOM
        //0b1010
        0b00000000001010000000000000000000,
        0b00000000011010000001000000000000,
        0b00000000111010000001000000000001,
        0b00000000101010000000000000000001,

        //TOP
        //0b0010
        0b00000000000010000000000001000000,
        0b00000000010010000000000001000001,
        0b00000000110010000001000001000001,
        0b00000000100010000001000001000000
    };


    m_VertexCount = 0;
    m_RenderData.m_IndexCount = 0;
    int offset = 0;
    for (int i = 0; i < CHUNK_VOLUME; i++)
    {
        int xPos = i % CHUNK_SIZE;
        int yPos = i / (CHUNK_SIZE * CHUNK_SIZE);
        int zPos = (i / CHUNK_SIZE) % CHUNK_SIZE;

        //        if (xPos > 0 && yPos > 0 && zPos > 0 && xPos < CHUNK_SIZE - 1 && yPos < CHUNK_SIZE - 1 && zPos < CHUNK_SIZE - 1)
        //            continue;


        if (m_Data[i] != 0)
        {
            AddBlock(i);
            /*
            for (int j = 0; j < 24; j++)
            {
                m_Mesh[m_VertexCount + j] = cube[j] + (m_Data[i] << 24) + (xPos << 12) + (yPos << 6) + zPos;
            }

            m_VertexCount += 24;
            m_RenderData.m_IndexCount += 36;*/
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