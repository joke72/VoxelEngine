#pragma once

#include <iostream>
#include <glad/glad.h>

#include "stb_image.h"

class Texture
{
public:
	uint32_t m_ID;
	uint32_t m_ActiveID;
	int32_t m_Width, m_Height, m_Channels;
	static uint32_t m_TotalTextures;

	Texture(int32_t width, int32_t height, uint32_t activeID, uint32_t binding);
	Texture(int32_t width, int32_t height, uint32_t activeID, uint32_t binding, void* start);
	Texture(const char* path, bool diffuse, uint32_t activeID);

	void Bind();
};

