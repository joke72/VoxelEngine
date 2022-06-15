#include "ChunkRenderer.h"

ChunkRenderer::ChunkRenderer() : 
	m_ChunkVertices(GenerateVertexArray()),
	m_ChunkIndices(GenerateIndexArray()),
	m_VAO(),
	m_VBO(m_ChunkVertices, GetMaxVertexCount() * sizeof(GLuint), GL_DYNAMIC_DRAW),
	m_IBO(m_ChunkIndices, GetMaxIndexCount() * sizeof(GLuint)),
    m_Layout(),
    m_IndexCount(GetMaxIndexCount()),
    m_VertexCount(GetMaxVertexCount())
{
	//BlockID, Normal and position
	m_Layout.AddVertexAttribute(GL_FLOAT, 1);

	m_VAO.AddBuffer(m_VBO, m_IBO, m_Layout);

    std::cout << "chunkrenderer intialized" << std::endl;
}

GLuint* ChunkRenderer::GenerateVertexArray()
{
    GLuint cube[] = {
        // 16 bits : ID, 4 bits : normals, 12 bits : chunk_position  

        // FAR
        //0b1001
        0x09000,
        0x09010,
        0x09110,
        0x09100,

        // NEAR
        //0b0001
        0x01001,
        0x01101,
        0x01111,
        0x01011,

        // LEFT
        //0b1100
        0x0C011,
        0x0C010,
        0x0C000,
        0x0C001,

        // RIGHT
        //0b0100
        0x04110,
        0x04111,
        0x04101,
        0x04100,

        // BOTTOM
        //0b1010
        0x0A000,
        0x0A100,
        0x0A101,
        0x0A001,

        //TOP
        //0b0010
        0x12010,
        0x12011,
        0x12111,
        0x12110
    };

	int vertexCount = GetMaxVertexCount();
	GLuint* vertices = new GLuint[vertexCount];
	for (int i = 0; i < vertexCount; i++)
	{
        int currentBlock = (i / 24);

        int xPos = currentBlock % CHUNK_SIZE;
        int yPos = currentBlock / (CHUNK_SIZE * CHUNK_SIZE);
        int zPos = (currentBlock / CHUNK_SIZE) % CHUNK_SIZE;

        vertices[i] = 0;// cube[i % 24] + (xPos << 8) + (yPos << 4) + zPos;

	}
    std::cout << "vertices generated" << std::endl;

	return vertices;
}

GLuint* ChunkRenderer::GenerateIndexArray()
{
    int indexCount = GetMaxIndexCount();
	GLuint* indices = new GLuint[indexCount];

    int offset = 0;
	for (int i = 0; i < indexCount; i++)
	{        

        indices[i] = 0;
        /*
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 2 + offset;

		indices[i + 3] = 2 + offset;
		indices[i + 4] = 3 + offset;
		indices[i + 5] = 0 + offset;
		
        offset += 4;
        */
        //std::cout << indices[i] << std::endl;
	}
    std::cout << "indices generated" << std::endl;
	return indices;
}

GLuint ChunkRenderer::GetMaxIndexCount() const
{
    return 24336*2;// CHUNK_VOLUME * 36;
}

GLuint ChunkRenderer::GetMaxVertexCount() const
{
	return CHUNK_VOLUME * 24;
}

void ChunkRenderer::RecalculateMesh(const GLuint* chunkData)
{
    GLuint cube[] = {
        // 16 bits : ID, 4 bits : normals, 12 bits : chunk_position  

        // FAR
        //0b1001
        0x09000,
        0x09010,
        0x09110,
        0x09100,

        // NEAR
        //0b0001
        0x01001,
        0x01101,
        0x01111,
        0x01011,

        // LEFT
        //0b1100
        0x0C011,
        0x0C010,
        0x0C000,
        0x0C001,

        // RIGHT
        //0b0100
        0x04110,
        0x04111,
        0x04101,
        0x04100,

        // BOTTOM
        //0b1010
        0x0A000,
        0x0A100,
        0x0A101,
        0x0A001,

        //TOP
        //0b0010
        0x02010,
        0x02011,
        0x02111,
        0x02110
    };

    m_VertexCount = 0;
    m_IndexCount = 0;
    int offset = 0;
    for (int i = 0; i < CHUNK_VOLUME; i++)
    {
        int xPos = i % CHUNK_SIZE;
        int yPos = i / (CHUNK_SIZE * CHUNK_SIZE);
        int zPos = (i / CHUNK_SIZE) % CHUNK_SIZE;

        if (xPos > 0 && yPos > 0 && zPos > 0 && xPos < CHUNK_SIZE - 1 && yPos < CHUNK_SIZE - 1 && zPos < CHUNK_SIZE - 1)
            continue;


        if (chunkData[i] == 0)
        {

            for (int j = 0; j < 24; j++)
            {
                m_ChunkVertices[m_VertexCount + j] = cube[j] + (chunkData[i] << 16) + (xPos << 8) + (yPos << 4) + zPos;
            }
            for (int i = m_IndexCount; i < m_IndexCount + 36; i += 6)
            {
                m_ChunkIndices[i + 0] = 0 + offset;
                m_ChunkIndices[i + 1] = 1 + offset;
                m_ChunkIndices[i + 2] = 2 + offset;

                m_ChunkIndices[i + 3] = 2 + offset;
                m_ChunkIndices[i + 4] = 3 + offset;
                m_ChunkIndices[i + 5] = 0 + offset;

                offset += 4;

            }
            m_VertexCount += 24;
            m_IndexCount += 36;
        }
    }
}

void ChunkRenderer::Render(const Chunk& chunk, const Shader& shader)
{
	shader.use();
    shader.setVec3("uChunkPosition", chunk.m_Position);
    RecalculateMesh(chunk.m_ChunkData);

    std::cout << m_IndexCount << std::endl;

    m_VAO.Bind();
    m_VBO.BufferData(m_ChunkVertices, m_VertexCount * sizeof(GLuint));
    m_IBO.BufferData(m_ChunkIndices, m_IndexCount * sizeof(GLuint));
	glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0);
}

ChunkRenderer::~ChunkRenderer()
{
	delete [] m_ChunkVertices;
	delete [] m_ChunkIndices;
}
