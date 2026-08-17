#include "Texture2D.h"
#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>
#include <utility> // for std::move

Texture2D::Texture2D(const std::string& path, const std::string& type, bool gammaCorrection)
	: m_Path(path), m_Type(type), m_GammaCorrection(gammaCorrection)
{
	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);

	if (stbi_is_hdr(path.c_str()))
	{
		float* data = stbi_loadf(path.c_str(), &width, &height, &channels, 0);
		if (data)
		{
			m_Width = width;
			m_Height = height;

			GLenum internalFormat = (channels == 4) ? GL_RGBA16F : GL_RGB16F;
			GLenum dataFormat = (channels == 4) ? GL_RGBA : GL_RGB;

			glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
			glTextureStorage2D(m_ID, 1, internalFormat, m_Width, m_Height);
			glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_FLOAT, data);

			glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load HDR texture: " << path << std::endl;
		}
	}
	else
	{
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		if (data)
		{
			m_Width = width;
			m_Height = height;

			GLenum internalFormat = 0, dataFormat = 0;
			if (channels == 1)
			{
				internalFormat = GL_R8;
				dataFormat = GL_RED;
			}
			else if (channels == 3)
			{
				internalFormat = m_GammaCorrection ? GL_SRGB8 : GL_RGB8;
				dataFormat = GL_RGB;
			}
			else if (channels == 4)
			{
				internalFormat = m_GammaCorrection ? GL_SRGB8_ALPHA8 : GL_RGBA8;
				dataFormat = GL_RGBA;
			}

			glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);

			// Pre-allocate memory on the GPU
			glTextureStorage2D(m_ID, 1, internalFormat, m_Width, m_Height);

			// Upload pixel data
			glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

			// Set filtering parameters
			glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glGenerateTextureMipmap(m_ID);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Failed to load texture: " << path << std::endl;
		}
	}

	
}

Texture2D::~Texture2D()
{
	Delete();
}

Texture2D::Texture2D(Texture2D&& other) noexcept
    : m_ID(other.m_ID)
    , m_Width(other.m_Width)
    , m_Height(other.m_Height)
    , m_Path(std::move(other.m_Path))
    , m_Type(std::move(other.m_Type))
    , m_GammaCorrection(other.m_GammaCorrection)
{
    other.m_ID = 0;
    other.m_Width = 0;
    other.m_Height = 0;
    other.m_GammaCorrection = false;
}

Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
{
    if (this != &other) {
        if (m_ID) {
            Delete();
        }

        m_ID = other.m_ID;
        m_Width = other.m_Width;
        m_Height = other.m_Height;
        m_Path = std::move(other.m_Path);
        m_Type = std::move(other.m_Type);
        m_GammaCorrection = other.m_GammaCorrection;

        other.m_ID = 0;
        other.m_Width = 0;
        other.m_Height = 0;
        other.m_GammaCorrection = false;
    }
    return *this;
}

void Texture2D::AllocateEmpty(uint32_t width, uint32_t height, GLenum internalFormat)
{
	if (m_ID)
	{
		Delete();
	}

	m_Width = width;
	m_Height = height;

	glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);

	// glTextureStorage2D allocates immutable storage for all 6 faces at once when given GL_TEXTURE_CUBE_MAP
	glTextureStorage2D(m_ID, 1, internalFormat, m_Width, m_Height);

	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	
}

void Texture2D::CreateFromData(const void* data, uint32_t width, uint32_t height,
	GLenum internalFormat, GLenum dataFormat, GLenum dataType,
	const std::string& typeName)
{
	if (!data)
	{
		std::cerr << "No data provided for texture creation." << std::endl;
		return;
	}

	if (m_ID)
	{
		std::cerr << "Texture already created. Deleting old texture: " << m_Path << std::endl;
		Delete();
	}

	m_Width = width;
	m_Height = height;
	m_Type = typeName;
	m_Path = "[raw data]";

	glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
	glTextureStorage2D(m_ID, 1, internalFormat, m_Width, m_Height);
	glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, dataFormat, dataType, data);

	glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Only generate mipmaps if you allocated mip levels in Storage
	// glGenerateTextureMipmap(m_ID);
}

void Texture2D::Bind(uint32_t slot) const
{
	// DSA equivalent of glActiveTexture + glBindTexture
	glBindTextureUnit(slot, m_ID);
}

void Texture2D::UnBind(uint32_t slot) const
{
	glBindTextureUnit(slot, 0);
}

void Texture2D::Delete()
{
	if (m_ID)
	{
		glDeleteTextures(1, &m_ID);
		m_ID = 0;
	}
}