#ifndef CUBED_SHADER_H
#define CUBED_SHADER_H

#include <glm/include/glm.hpp>
#define GLEW_STATIC
#include <glew/include/glew.h>
#include <string>
#include <vector>
#include <utility>
#include "uniform.h"

typedef std::pair<UniformType, std::string> UniformDeclaration;

class RenderingEngine;

class Shader
{
public:
	Shader(const std::string& filename, const std::vector<std::string>& attributes, const std::vector<UniformDeclaration>& uniforms);
	~Shader();

	void bind() const;
	void update_uniforms(RenderingEngine& re);

private:
	static void check_shader_error(GLuint shader, GLuint flag, bool is_program, const std::string& error_message);
	static std::string load_shader(const std::string& filename);
	static GLuint create_shader(const std::string& text, GLenum shader_type);

	enum
	{
		VERTEX_SHADER,
		FRAGMENT_SHADER,
		NUM_SHADERS
	};

	GLuint m_program;
	GLuint m_shaders[NUM_SHADERS];
	std::vector<Uniform> m_uniforms;
};

#include "cubed_exception.h"

class ShaderException : public CubedException
{
public:
	ShaderException(std::string message) : CubedException(std::move(message)) { }
};

#endif