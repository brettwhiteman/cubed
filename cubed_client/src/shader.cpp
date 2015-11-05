#include "shader.h"
#include <fstream>
#include <iterator>
#include "rendering_engine.h"

shader::shader(const std::string& filename, const std::vector<std::string>& attributes, const std::vector<uniform_declaration>& uniforms)
{
	m_program = glCreateProgram();

	m_shaders[VERTEX_SHADER] = create_shader(load_shader("res/shaders/" + filename + ".vs"), GL_VERTEX_SHADER);
	m_shaders[FRAGMENT_SHADER] = create_shader(load_shader("res/shaders/" + filename + ".fs"), GL_FRAGMENT_SHADER);

	glAttachShader(m_program, m_shaders[VERTEX_SHADER]);
	glAttachShader(m_program, m_shaders[FRAGMENT_SHADER]);

	int i = 0;
	for(auto& attribute : attributes)
	{
		glBindAttribLocation(m_program, i++, attribute.c_str());
	}

	glLinkProgram(m_program);
	check_shader_error(m_program, GL_LINK_STATUS, true, "Shader program linking failed");

	glValidateProgram(m_program);
	check_shader_error(m_program, GL_VALIDATE_STATUS, true, "Shader program is invalid");

	for (auto& uniform : uniforms)
	{
		m_uniforms.emplace_back(uniform.first, uniform.second, glGetUniformLocation(m_program, uniform.second.c_str()));
	}
}

shader::~shader()
{
	glDetachShader(m_program, m_shaders[VERTEX_SHADER]);
	glDeleteShader(m_shaders[VERTEX_SHADER]);

	glDetachShader(m_program, m_shaders[FRAGMENT_SHADER]);
	glDeleteShader(m_shaders[FRAGMENT_SHADER]);

	glDeleteProgram(m_program);
}

void shader::bind() const
{
	glUseProgram(m_program);
}

void shader::update_uniforms(rendering_engine& re)
{
	for(auto uniform : m_uniforms)
	{
		uniform.update(re);
	}
}

std::string shader::load_shader(const std::string& filename)
{
	std::ifstream file(filename);

	if(!file.is_open())
	{
		throw shader_exception("Failed to load shader file: " + filename);
	}

	std::noskipws(file);

	std::istream_iterator<char> start(file);
	std::string source(start, std::istream_iterator<char>());

	if(source.empty())
	{
		throw shader_exception("Failed to load shader file: " + filename);
	}

	return source;
}

GLuint shader::create_shader(const std::string& source, GLenum shader_type)
{
	GLuint shader = glCreateShader(shader_type);

	if(!shader)
	{
		throw shader_exception("Shader creation failed");
	}

	const GLchar* source_strings[1];
	GLint source_string_lengths[1];

	source_strings[0] = source.c_str();
	source_string_lengths[0] = source.length();

	glShaderSource(shader, 1, source_strings, source_string_lengths);

	glCompileShader(shader);

	check_shader_error(shader, GL_COMPILE_STATUS, false, "Shader compilation failed");

	return shader;
}

void shader::check_shader_error(GLuint shader, GLuint flag, bool is_program, const std::string& error_message)
{
	GLint success = 0;
	GLchar error[1024] = {0};

	if(is_program)
	{
		glGetProgramiv(shader, flag, &success);
	}
	else
	{
		glGetShaderiv(shader, flag, &success);
	}

	if(success == GL_FALSE)
	{
		if(is_program)
		{
			glGetProgramInfoLog(shader, sizeof(error), 0, error);
		}
		else
		{
			glGetShaderInfoLog(shader, sizeof(error), 0, error);
		}

		throw shader_exception(error_message + ": " + error);
	}
}