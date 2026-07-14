#pragma once
#include <cstdint>
#include <vector>

enum class FramebufferTextureFormat
{
	None = 0,

	// Color
	RGBA8,
	RED_INTEGER,

	// Depth/stencil
	DEPTH24STENCIL8,

	// Defaults
	Depth = DEPTH24STENCIL8
};

struct FramebufferTextureSpecification
{
	FramebufferTextureSpecification() = default;
	FramebufferTextureSpecification(FramebufferTextureFormat format)
		: TextureFormat(format) {
	}

	FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
};

struct FramebufferAttachmentSpecification
{
	FramebufferAttachmentSpecification() = default;
	FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
		: Attachments(attachments) {
	}

	std::vector<FramebufferTextureSpecification> Attachments;
};

struct FramebufferSpecification
{
	uint32_t Width, Height;
	FramebufferAttachmentSpecification Attachments;
	uint32_t Samples = 1;
};

class FrameBuffer
{
public:
	FrameBuffer() = default;
	FrameBuffer(const FramebufferSpecification& spec);
	~FrameBuffer();
	FrameBuffer(const FrameBuffer&) = delete;
	FrameBuffer& operator=(const FrameBuffer&) = delete;

	void Bind() const;
	void UnBind() const;

	void Resize(uint32_t width, uint32_t height);

	const FramebufferSpecification& GetSpecification() const { return m_Specification; }
	uint32_t GetColorAttachment(uint32_t index = 0) const;

	static void ReadFromReadTo(const uint32_t readFrom, const uint32_t writeTo);

private:
	void Invalidate();

private:
	uint32_t m_ID = 0;
	FramebufferSpecification m_Specification;

	std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
	FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;

	std::vector<uint32_t> m_ColorAttachments;
	uint32_t m_DepthAttachment = 0;
};

