#include "TextureCubeMap.h"

#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>
#include <utility> // for std::move

TextureCubeMap::TextureCubeMap(const std::vector<std::string>& paths) : m_Paths(paths)
{
	if (paths.size() != 6)
	{
		std::cerr << "TextureCubeMap requires exactly 6 face paths, got " << paths.size() << std::endl;
		return;
	}

	glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_ID);

	int width = 0, height = 0, channels = 0;
	bool storageAllocated = false;

	for (size_t i = 0; i < 6; ++i)
	{
		stbi_set_flip_vertically_on_load(false);

		unsigned char* data = stbi_load(paths[i].c_str(), &width, &height, &channels, 0);
		if (!data)
		{
			std::cerr << "Failed to load cubemap face [" << i << "]: " << paths[i] << std::endl;
			continue;
		}

		if (!storageAllocated)
		{
			m_Width = static_cast<uint32_t>(width);
			m_Height = static_cast<uint32_t>(height);

			if (m_Width != m_Height)
			{
				std::cerr << "Warning: Cubemap face " << i << " is not square ("
					<< m_Width << "x" << m_Height << "). Cube maps should use square textures." << std::endl;
			}

			GLenum internalFormat = GL_RGB8;
			if (channels == 1)
				internalFormat = GL_R8;
			else if (channels == 3)
				internalFormat = GL_RGB8;
			else if (channels == 4)
				internalFormat = GL_RGBA8;

			// Calculate mipmap levels for the allocated storage
			int levels = static_cast<int>(std::floor(std::log2(std::max(m_Width, m_Height)))) + 1;

			// Allocate immutable storage for all 6 faces
			glTextureStorage2D(m_ID, levels, internalFormat, m_Width, m_Height);
			storageAllocated = true;
		}

		GLenum dataFormat = GL_RGB;
		if (channels == 1)
			dataFormat = GL_RED;
		else if (channels == 3)
			dataFormat = GL_RGB;
		else if (channels == 4)
			dataFormat = GL_RGBA;

		// Upload to specific cube face using DSA.
		// zoffset = face index: 0=+X, 1=-X, 2=+Y, 3=-Y, 4=+Z, 5=-Z
		glTextureSubImage3D(m_ID, 0, 0, 0, static_cast<GLint>(i), width, height, 1, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	if (storageAllocated)
	{
		// Generate mipmaps for all faces
		glGenerateTextureMipmap(m_ID);

		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
	else
	{
		std::cerr << "TextureCubeMap: No faces were loaded successfully." << std::endl;
	}


}

TextureCubeMap::~TextureCubeMap()
{
	Delete();
}

TextureCubeMap::TextureCubeMap(TextureCubeMap&& other) noexcept
	: m_ID(other.m_ID)
	, m_Width(other.m_Width)
	, m_Height(other.m_Height)
	, m_Paths(std::move(other.m_Paths))
{
	other.m_ID = 0;
	other.m_Width = 0;
	other.m_Height = 0;
}

TextureCubeMap& TextureCubeMap::operator=(TextureCubeMap&& other) noexcept
{
	if (this != &other)
	{
		if (m_ID)
		{
			Delete();
		}

		m_ID = other.m_ID;
		m_Width = other.m_Width;
		m_Height = other.m_Height;
		m_Paths = std::move(other.m_Paths);

		other.m_ID = 0;
		other.m_Width = 0;
		other.m_Height = 0;
	}
	return *this;
}

void TextureCubeMap::AllocateEmpty(uint32_t width, uint32_t height, GLenum internalFormat, bool enableMipmaps)
{
	if (m_ID)
	{
		Delete();
	}

	m_Width = width;
	m_Height = height;

	glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_ID);

	// Calculate mipmap levels (necessary for pre-filtered specular PBR cubemaps)
	int levels = enableMipmaps ? static_cast<int>(std::floor(std::log2(std::max(m_Width, m_Height)))) + 1 : 1;

	// glTextureStorage2D allocates immutable storage for all 6 faces at once when given GL_TEXTURE_CUBE_MAP
	glTextureStorage2D(m_ID, levels, internalFormat, m_Width, m_Height);

	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (enableMipmaps)
	{
		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateTextureMipmap(m_ID);
	}
	else
	{
		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
}

void TextureCubeMap::Bind(uint32_t slot) const
{
	// DSA equivalent of glActiveTexture + glBindTexture
	glBindTextureUnit(slot, m_ID);
}

void TextureCubeMap::UnBind(uint32_t slot) const
{
	glBindTextureUnit(slot, 0);
}

void TextureCubeMap::Delete()
{
	if (m_ID)
	{
		glDeleteTextures(1, &m_ID);
		m_ID = 0;
	}
}