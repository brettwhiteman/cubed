#ifndef CUBED_TEXTURE_H
#define CUBED_TEXTURE_H

#define GLEW_STATIC
#include <glew/include/glew.h>
#include <string>

class Texture
{
public:
	Texture(const std::string& filename);
	Texture(const unsigned char* pixels, int width, int height);
	Texture(const Texture&) = delete;
	~Texture();

	void bind();

private:
	GLuint m_texture;
};

#include "cubed_exception.h"
#include <utility>

class TextureException : public CubedException
{
public:
	TextureException(std::string message) : CubedException(std::move(message)) { }
};

#endif