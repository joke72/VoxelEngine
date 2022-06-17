#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


struct FramebufferSpecification {
	uint32_t Width;
	uint32_t Height;
	uint32_t InternalTextureFormat;
	uint32_t TextureFormat;
	uint32_t Attachment;
	uint32_t Type;
};


class Framebuffer
{
private:
	FramebufferSpecification m_Specification;
	uint32_t m_FramebufferID;
	uint32_t m_TextureID;
	uint32_t m_RenderBufferID;

public:
	Framebuffer(FramebufferSpecification specification);

	~Framebuffer();
	void Bind();
	void Unbind();

	void BindTexture();
	void UnbindTexture();

	void AddRenderBuffer(uint32_t internalFormat, uint32_t attachment);
};

