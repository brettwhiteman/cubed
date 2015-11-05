#ifndef CUBED_SHADER_H
#define CUBED_SHADER_H

#include <glm/include/glm.hpp>
#define GLEW_STATIC
#include <glew/include/glew.h>
#include <string>
#include <vector>
#include <utility>
#include "uniform.h"

typedef std::pair<uniform_type, std::string> uniform_declaration;

class rendering_engine;

class shader
{
public:
	shader(const std::string& filename, const std::vector<std::string>& attributes, const std::vector<uniform_declaration>& uniforms);
	~shader();

	void bind() const;
	void update_uniforms(rendering_engine& re);

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
	std::vector<uniform> m_uniforms;
};

#include "cubed_exception.h"

class shader_exception : public cubed_exception
{
public:
	shader_exception(std::string message) : cubed_exception(std::move(message)) { }
};

#endif