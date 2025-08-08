#pragma once
#include "Texture.h"


unsigned int Texture::m_TotalTextures = 0;

Texture::Texture(int32_t width, int32_t height, uint32_t activeID, uint32_t binding) :
	m_Width(width),
	m_Height(height),
	m_Channels(3),
	m_ActiveID(activeID)
{
	glGenTextures(1, &m_ID);
	glActiveTexture(GL_TEXTURE0 + m_ActiveID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);

	glBindImageTexture(binding, m_ID, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

	//glBindTexture(GL_TEXTURE_2D, 0);
}
Texture::Texture(int32_t width, int32_t height, uint32_t activeID, uint32_t binding, void* start) :
	m_Width(width),
	m_Height(height),
	m_Channels(3),
	m_ActiveID(activeID)
{
	glGenTextures(1, &m_ID);
	glActiveTexture(GL_TEXTURE0 + m_ActiveID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, start);

	glBindImageTexture(binding, m_ID, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

	//glBindTexture(GL_TEXTURE_2D, 0);
}


void placeWithOffset(uint8_t* data, uint8_t* newData, uint32_t width, uint32_t height, uint32_t newWidth, uint32_t newHeight, uint32_t m_Channels, uint32_t xOffset, uint32_t yOffset, uint32_t xFrom, uint32_t yFrom, uint32_t tilesX, uint32_t areaWidth, uint32_t areaHeight)
{
	tilesX = tilesX % 4;

	uint8_t* start;
	uint8_t* startNew;

	start = data;

	//std::cout << &(newData) << std::endl;

	for (int y = 0; y < areaHeight; y++)
	{
		for (int i = 0; i < areaWidth * m_Channels; i++)
		{
			*(newData + (newWidth * (y + yOffset) + xOffset) * m_Channels + tilesX * (y + yOffset) + i) = *(data + (width * (y + yFrom) + xFrom) * m_Channels + 0 * (y + yFrom) + i);
		}
	}
}


Texture::Texture(const char* path, bool diffuse, uint32_t activeID) :
	m_ActiveID(activeID)
{
	glGenTextures(1, &m_ID);
	glActiveTexture(GL_TEXTURE0 + m_ActiveID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_set_flip_vertically_on_load(true);
	uint8_t* data = (uint8_t*) stbi_load(path, &m_Width, &m_Height, &m_Channels, 0);
	//std::cout << m_Height << std::endl;

//	std::cout << m_Channels << std::endl;
	/*
	const int size = m_Width * m_Height;

	int tileSize = 16;

	int tilesX = m_Width / tileSize;
	int tilesY = m_Height / tileSize;
	std::cout << tilesX << std::endl;

	uint32_t newWidth = (m_Width + tilesX*2);
	uint32_t newHeight = (m_Height + tilesY*2);

	uint8_t* newData = new uint8_t[newWidth * newHeight * m_Channels];

	int x = 0;
	int y = 0;
	uint32_t padOffsetX = 0;

	for (int i = 0; i < newWidth * newHeight; i++)
		newData[i] = 0;
	for (int x = 0; x < tilesX; x++)
	{
		padOffsetX++;
		uint32_t padOffsetY = 0;
		for (int y = 0; y < tilesY; y++)
		{
			padOffsetY++;
			placeWithOffset(data, newData, m_Width, m_Height, newWidth, newHeight, m_Channels, padOffsetX + x * tileSize, padOffsetY + y * tileSize - 1, x * tileSize, y * tileSize, tilesX, 16, 1);
			
			placeWithOffset(data, newData, m_Width, m_Height, newWidth, newHeight, m_Channels, padOffsetX + x * tileSize - 1, padOffsetY + y * tileSize, x * tileSize, y * tileSize, tilesX, 1, 16);

			placeWithOffset(data, newData, m_Width, m_Height, newWidth, newHeight, m_Channels, padOffsetX + (x + 1) * tileSize, padOffsetY + y * tileSize, (x+1) * tileSize-1, y * tileSize, tilesX, 1, 16);
			placeWithOffset(data, newData, m_Width, m_Height, newWidth, newHeight, m_Channels, padOffsetX + x       * tileSize, padOffsetY + (y + 1) * tileSize, x * tileSize, (y+1) * tileSize-1, tilesX, 16, 1);

			
			placeWithOffset(data, newData, m_Width, m_Height, newWidth, newHeight, m_Channels, padOffsetX + x * tileSize, padOffsetY + y * tileSize, x*tileSize, y*tileSize, tilesX, 16, 16);
			padOffsetY++;
		}
		padOffsetX++;
	}*/

	if (data)
	{
		if (diffuse)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, m_Width, m_Height, 0, m_Channels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, m_Channels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	//delete[] newData;
	glBindTexture(GL_TEXTURE_2D, 0);

}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + m_ActiveID);
	glBindTexture(GL_TEXTURE_2D, m_ID);
}


