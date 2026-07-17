#include "FrameBuffer.h"
#include <glad/glad.h>
#include <iostream>
#include <cassert>
#include <print>

static const uint32_t s_MaxFramebufferSize = 8192;

namespace Utils
{
	static GLenum GetGLTextureTarget(TextureTarget target, bool isMultisampled)
	{
		if (target == TextureTarget::TextureCubeMap) return GL_TEXTURE_CUBE_MAP;
		return isMultisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}

	static void CreateTextures(TextureTarget target ,bool isMultisampled, uint32_t count, uint32_t* outID)
	{
		glCreateTextures(GetGLTextureTarget(target ,isMultisampled), count, outID);
	}

	static void BindTexture(TextureTarget target ,bool isMultisampled, uint32_t id)
	{
		glBindTexture(GetGLTextureTarget(target, isMultisampled), id);
	}


	static void AttachColorTexture(uint32_t fboID, uint32_t texID, int samples, GLenum internalFormat, uint32_t width, uint32_t height, int index)
	{
		bool isMultisampled = samples > 1;

		if (isMultisampled)
		{
			glTextureStorage2DMultisample(texID, samples, internalFormat, width, height, GL_FALSE);
		}
		else
		{
			// glTextureStorage2D intrinsically handles all 6 faces if the target was created as GL_TEXTURE_CUBE_MAP
			glTextureStorage2D(texID, 1, internalFormat, width, height);

			GLenum filter = (internalFormat == GL_R32I) ? GL_NEAREST : GL_LINEAR;
			glTextureParameteri(texID, GL_TEXTURE_MIN_FILTER, filter);
			glTextureParameteri(texID, GL_TEXTURE_MAG_FILTER, filter);
			glTextureParameteri(texID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTextureParameteri(texID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(texID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glNamedFramebufferTexture(fboID, GL_COLOR_ATTACHMENT0 + index, texID, 0);
	}

	static void AttachDepthTexture(uint32_t fboID, uint32_t texID, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
	{
		bool isMultisampled = samples > 1;

		if (isMultisampled)
		{
			glTextureStorage2DMultisample(texID, samples, format, width, height, GL_FALSE);
		}
		else
		{
			glTextureStorage2D(texID, 1, format, width, height);

			glTextureParameteri(texID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(texID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTextureParameteri(texID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTextureParameteri(texID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(texID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glNamedFramebufferTexture(fboID, attachmentType, texID, 0);
	}

	static bool IsDepthFormat(FramebufferTextureFormat format)
	{
		switch (format)
		{
		case FramebufferTextureFormat::DEPTH24STENCIL8: return true;
		}

		return false;
	}
}


FrameBuffer::FrameBuffer(const FramebufferSpecification& spec) : m_Specification(spec)
{	

	for (auto spec : m_Specification.Attachments.Attachments)
	{
		if (!Utils::IsDepthFormat(spec.TextureFormat))
		{
			m_ColorAttachmentSpecifications.emplace_back(spec);
		}
		else
		{
			m_DepthAttachmentSpecification = spec;
		}
	}

	Invalidate();

}

FrameBuffer::FrameBuffer(FrameBuffer&& other) noexcept
	: m_ID(std::exchange(other.m_ID, 0)),
	m_Specification(std::move(other.m_Specification)),
	m_ColorAttachmentSpecifications(std::move(other.m_ColorAttachmentSpecifications)),
	m_DepthAttachmentSpecification(std::move(other.m_DepthAttachmentSpecification)),
	m_ColorAttachments(std::move(other.m_ColorAttachments)),
	m_DepthAttachment(std::exchange(other.m_DepthAttachment, 0))
{
}

FrameBuffer& FrameBuffer::operator=(FrameBuffer&& other) noexcept
{
	if (this != &other)
	{
		if (m_ID)
		{
			glDeleteFramebuffers(1, &m_ID);
			glDeleteTextures(static_cast<GLsizei>(m_ColorAttachments.size()), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);
		}

		m_ID = std::exchange(other.m_ID, 0);
		m_Specification = std::move(other.m_Specification);
		m_ColorAttachmentSpecifications = std::move(other.m_ColorAttachmentSpecifications);
		m_DepthAttachmentSpecification = std::move(other.m_DepthAttachmentSpecification);
		m_ColorAttachments = std::move(other.m_ColorAttachments);
		m_DepthAttachment = std::exchange(other.m_DepthAttachment, 0);
	}
	return *this;
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &m_ID);
	glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
	glDeleteTextures(1, &m_DepthAttachment);

	m_ColorAttachments.clear();
	m_DepthAttachment = 0;
}

void FrameBuffer::Invalidate()
{
	// IF the framebuffer already exists, delete it and its attachments
	if (m_ID)
	{
		glDeleteFramebuffers(1, &m_ID);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);

		m_ColorAttachments.clear();
		m_DepthAttachment = 0;

	}

	glCreateFramebuffers(1, &m_ID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

	bool isMultisampled = m_Specification.Samples > 1;
	

	// Color Attachments
	if (m_ColorAttachmentSpecifications.size())
	{
		m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());

		for (size_t i = 0; i < m_ColorAttachments.size(); i++)
		{
			TextureTarget target = m_Specification.Attachments.Attachments[i].TextureTarget;

			Utils::CreateTextures(target, isMultisampled, 1, &m_ColorAttachments[i]);
			switch (m_ColorAttachmentSpecifications[i].TextureFormat)
			{
			case FramebufferTextureFormat::RGBA8:
				Utils::AttachColorTexture(m_ID, m_ColorAttachments[i], m_Specification.Samples, GL_RGBA8, m_Specification.Width, m_Specification.Height, i);
				break;
			case FramebufferTextureFormat::RED_INTEGER:
				Utils::AttachColorTexture(m_ID, m_ColorAttachments[i], m_Specification.Samples, GL_R32I, m_Specification.Width, m_Specification.Height, i);
				break;
			}
		}
	}

	if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
	{
		TextureTarget target = m_DepthAttachmentSpecification.TextureTarget;
		Utils::CreateTextures(target, isMultisampled, 1, &m_DepthAttachment);
		switch (m_DepthAttachmentSpecification.TextureFormat)
		{
		case FramebufferTextureFormat::DEPTH24STENCIL8:
			Utils::AttachDepthTexture(m_ID, m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
			break;
		}
	}

	if (m_ColorAttachments.size() > 1)
	{
		assert(m_ColorAttachments.size() <= 4);
		GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(m_ColorAttachments.size(), buffers);
	}
	else if (m_ColorAttachments.empty())
	{
		// Only depth-pass
		glNamedFramebufferDrawBuffer(m_ID, GL_NONE);
		glNamedFramebufferReadBuffer(m_ID, GL_NONE);
	}

	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE && "ERROR::FRAMEBUFFER:: Framebuffer is not complete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
}

void FrameBuffer::UnBind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::BindFace(uint32_t faceIndex) const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
	if (!m_ColorAttachments.empty())
	{
		glNamedFramebufferTextureLayer(m_ID, GL_COLOR_ATTACHMENT0, m_ColorAttachments[0], 0, faceIndex);
	}
}

void FrameBuffer::Resize(uint32_t width, uint32_t height)
{

	if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
	{
		std::println("Attempted to resize framebuffer to {0}, {1}", width, height);
		return;
	}

	if (width == m_Specification.Width && height == m_Specification.Height) return;

	m_Specification.Width = width;
	m_Specification.Height = height;

	Invalidate();
}

uint32_t FrameBuffer::GetColorAttachment(uint32_t index) const
{
	assert(index < m_ColorAttachments.size() && "Index out of bounds!");
	return m_ColorAttachments[index];
}

void FrameBuffer::ReadFromReadTo(const uint32_t readFrom, const uint32_t writeTo, const uint32_t width, const uint32_t height)
{
	glBlitNamedFramebuffer(readFrom, writeTo, 0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}