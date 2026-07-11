#include "Texture2D.h"
#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>

Texture2D::Texture2D(const std::string& path, const std::string& type)
	: m_Path(path), m_Type(type)
{
	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);

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
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		else if (channels == 4)
		{
			internalFormat = GL_RGBA8;
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

Texture2D::~Texture2D()
{
	Delete();
}

Texture2D::Texture2D(Texture2D&& other) noexcept
	: m_ID(other.m_ID), m_Width(other.m_Width), m_Height(other.m_Height), m_Path(std::move(other.m_Path)), m_Type(std::move(other.m_Type))
{
	other.m_ID = 0;
}

Texture2D& Texture2D::operator=(Texture2D&& other) noexcept
{
	if (this != &other)
	{
		Delete();
		m_ID = other.m_ID;
		m_Width = other.m_Width;
		m_Height = other.m_Height;
		m_Path = std::move(other.m_Path);
		m_Type = std::move(other.m_Type);

		other.m_ID = 0;
	}
	return *this;
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