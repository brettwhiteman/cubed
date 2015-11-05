#ifndef CUBED_RENDERING_ENGINE_H
#define CUBED_RENDERING_ENGINE_H

#include <string>
#include <unordered_map>
#include <glm/include/glm.hpp>
#include "shader.h"

class texture;
class window;

class rendering_engine
{
public:
	rendering_engine(window& window);
	rendering_engine(const rendering_engine&) = delete;
	~rendering_engine();

	void update_uniforms();
	void clear();

	void load_shader(std::string name, const std::vector<std::string>& attributes, const std::vector<uniform_declaration>& uniforms);
	void use_shader(const std::string& name);
	void load_texture(std::string name);
	void use_texture(const std::string& name);

	void set_vec4(std::string name, glm::vec4 value);
	const glm::vec4& get_vec4(const std::string& name);
	void set_mat4(std::string name, glm::mat4 value);
	const glm::mat4& get_mat4(const std::string& name);

	const glm::mat4& get_projection_matrix() { return m_projection; }

private:
	std::unordered_map<std::string, shader*> m_shaders;
	std::unordered_map<std::string, texture*> m_textures;
	std::unordered_map<std::string, glm::vec4> m_vec4_vars;
	std::unordered_map<std::string, glm::mat4> m_mat4_vars;
	const float PERSPECTIVE_ASPECT;
	const float PERSPECTIVE_Z_NEAR;
	const float PERSPECTIVE_Z_FAR;
	glm::mat4 m_projection;
};

#include "cubed_exception.h"

class rendering_engine_exception : public cubed_exception
{
public:
	rendering_engine_exception(std::string message) : cubed_exception(std::move(message)) { }
};

#endif