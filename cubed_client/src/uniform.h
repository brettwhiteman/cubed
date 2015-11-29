#ifndef CUBED_UNIFORM_H
#define CUBED_UNIFORM_H

#define GLEW_STATIC
#include <glew/include/glew.h>
#include <glm/include/glm.hpp>
#include <string>

enum UniformType
{
	UNIFORMTYPE_VEC4,
	UNIFORMTYPE_MAT4
};

class RenderingEngine;

class Uniform
{
public:
	Uniform(UniformType type, std::string name, GLint location) :
		m_type(type),
		m_name(std::move(name)),
		m_location(location)
	{
	}

	void update(RenderingEngine& re);

private:
	void set_value(const glm::vec4& value);
	void set_value(const glm::mat4& value);

	UniformType m_type;
	std::string m_name;
	GLint m_location;
};

#endif