#include "texture.h"
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/include/stb_image.h>

Texture::Texture(const std::string& filename)
{
	std::string path = "res\\textures\\" + filename;

	int w;
	int h;
	int comp;

	unsigned char* image = stbi_load(path.c_str(), &w, &h, &comp, STBI_rgb_alpha);

	if(!image)
	{
		throw TextureException("Failed to load texture: " + path);
	}

	glGenTextures(1, &m_texture);

	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(image);
}

Texture::Texture(const unsigned char* pixels, int width, int height)
{
	glGenTextures(1, &m_texture);

	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glBindTexture(GL_TEXTURE_2D, 0);

	glDeleteTextures(1, &m_texture);
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
}