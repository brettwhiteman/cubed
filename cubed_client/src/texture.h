#ifndef CUBED_TEXTURE_H
#define CUBED_TEXTURE_H

#define GLEW_STATIC
#include <glew/include/glew.h>
#include <string>

class texture
{
public:
	texture(const std::string& filename);
	texture(const unsigned char* pixels, int width, int height);
	texture(const texture&) = delete;
	~texture();

	void bind();

private:
	GLuint m_texture;
};

#include "cubed_exception.h"

class texture_exception : public cubed_exception
{
public:
	texture_exception(std::string message) : cubed_exception(std::move(message)) { }
};

#endif