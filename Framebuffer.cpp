#include "Framebuffer.h"


Framebuffer::Framebuffer(FramebufferSpecification specification) : 
	m_Specification(specification)
{
	glGenFramebuffers(1, &m_FramebufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);

	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, m_Specification.TextureFormat, m_Specification.Width, m_Specification.Height, 0, m_Specification.TextureFormat, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, m_Specification.Attachment, GL_TEXTURE_2D, m_TextureID, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::AddRenderBuffer(uint32_t internalFormat, uint32_t attachment)
{
	Bind();
	glGenRenderbuffers(1, &m_RenderBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, m_Specification.Width, m_Specification.Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, m_RenderBufferID);
	Unbind();
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Framebuffer::BindTexture()
{
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
}
void Framebuffer::UnbindTexture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);
}
void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &m_FramebufferID);
}