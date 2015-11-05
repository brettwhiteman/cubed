#ifndef CUBED_UNIFORM_H
#define CUBED_UNIFORM_H

#include <string>
#include <utility>
#define GLEW_STATIC
#include <glew/include/glew.h>
#include <glm/include/glm.hpp>

enum uniform_type
{
	UNIFORMTYPE_VEC4,
	UNIFORMTYPE_MAT4
};

class rendering_engine;

class uniform
{
public:
	uniform(uniform_type type, std::string name, GLint location);

	void update(rendering_engine& re);

private:
	void set_value(const glm::vec4& value);
	void set_value(const glm::mat4& value);

	uniform_type m_type;
	std::string m_name;
	GLint m_location;
};

#endif