#pragma once
#include <cstdint>
#include <vector>

enum class TextureTarget
{
	none = 0,

	Texture2D,
	TextureCubeMap,
};

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
	FramebufferTextureSpecification(FramebufferTextureFormat format, TextureTarget textureTarget = TextureTarget::Texture2D)
		: TextureFormat(format), TextureTarget(textureTarget) {
	}
	
	FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
	TextureTarget TextureTarget = TextureTarget::Texture2D;
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
	FrameBuffer(FrameBuffer&& other) noexcept;
	FrameBuffer& operator=(FrameBuffer&& other) noexcept;

	void Bind() const;
	void UnBind() const;

	void BindFace(uint32_t faceIndex) const;

	void Resize(uint32_t width, uint32_t height);

	const FramebufferSpecification& GetSpecification() const { return m_Specification; }
	uint32_t GetColorAttachment(uint32_t index = 0) const;
	uint32_t GetDepthAttachment() const { return m_DepthAttachment; }

	static void ReadFromReadTo(const uint32_t readFrom, const uint32_t writeTo, const uint32_t width, const uint32_t height);

	uint32_t GetID() const { return m_ID; }

private:
	void Invalidate();

private:
	uint32_t m_ID = 0;
	FramebufferSpecification m_Specification;

	std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications;
	FramebufferTextureSpecification m_DepthAttachmentSpecification{ FramebufferTextureFormat::None, TextureTarget::none};

	std::vector<uint32_t> m_ColorAttachments;
	uint32_t m_DepthAttachment = 0;
};

