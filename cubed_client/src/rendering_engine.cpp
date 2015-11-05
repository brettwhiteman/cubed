#include "rendering_engine.h"
#define GLEW_STATIC
#include <glew/include/glew.h>
#include "texture.h"
#include <glm/include/gtc/matrix_transform.hpp>
#include "window.h"

rendering_engine::rendering_engine(window& window)
	: PERSPECTIVE_ASPECT(glm::pi<float>() / 3.0f),
	PERSPECTIVE_Z_NEAR(0.05f),
	PERSPECTIVE_Z_FAR(1000.0f)
{
	if(glewInit() != GLEW_OK)
	{
		throw rendering_engine_exception("glewInit() failed");
	}

	glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
	glClearDepth(1.0f);

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	auto window_size = window.get_window_size();
	m_projection = glm::perspective(PERSPECTIVE_ASPECT, static_cast<float>(window_size.first) / window_size.second, PERSPECTIVE_Z_NEAR, PERSPECTIVE_Z_FAR);
	window.add_resize_handler([this](auto new_size)
	{
		m_projection = glm::perspective(PERSPECTIVE_ASPECT, static_cast<float>(new_size.first) / new_size.second, PERSPECTIVE_Z_NEAR, PERSPECTIVE_Z_FAR);
	});
}

rendering_engine::~rendering_engine()
{
	for(auto shader : m_shaders)
	{
		delete shader.second;
	}

	for (auto texture : m_textures)
	{
		delete texture.second;
	}
}

void rendering_engine::update_uniforms()
{
	for(auto shader : m_shaders)
	{
		shader.second->update_uniforms(*this);
	}
}

void rendering_engine::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void rendering_engine::load_shader(std::string name, const std::vector<std::string>& attributes, const std::vector<uniform_declaration>& uniforms)
{
	try
	{
		auto s = new shader(name, attributes, uniforms);
		m_shaders[std::move(name)] = s;
	}
	catch(const shader_exception& e)
	{
		throw rendering_engine_exception(e.what());
	}	
}

void rendering_engine::use_shader(const std::string& name)
{
	auto it = m_shaders.find(name);

	if(it == m_shaders.end())
	{
		throw rendering_engine_exception("The shader '" + name + "' doesn't exist");
	}

	it->second->bind();
}

void rendering_engine::load_texture(std::string name)
{
	try
	{
		auto t = new texture(name);
		m_textures[std::move(name)] = t;
	}
	catch (const texture_exception& e)
	{
		throw rendering_engine_exception(e.what());
	}
}

void rendering_engine::use_texture(const std::string& name)
{
	auto it = m_textures.find(name);

	if (it == m_textures.end())
	{
		throw rendering_engine_exception("The texture '" + name + "' doesn't exist");
	}

	it->second->bind();
}

void rendering_engine::set_vec4(std::string name, glm::vec4 value)
{
	m_vec4_vars[std::move(name)] = std::move(value);
}

const glm::vec4& rendering_engine::get_vec4(const std::string& name)
{
	auto it = m_vec4_vars.find(name);

	if(it == m_vec4_vars.end())
	{
		throw rendering_engine_exception("Unable to resolve rendering var '" + name + "'");
	}

	return it->second;
}

void rendering_engine::set_mat4(std::string name, glm::mat4 value)
{
	m_mat4_vars[std::move(name)] = std::move(value);
}

const glm::mat4& rendering_engine::get_mat4(const std::string& name)
{
	auto it = m_mat4_vars.find(name);

	if(it == m_mat4_vars.end())
	{
		throw rendering_engine_exception("Unable to resolve rendering var '" + name + "'");
	}

	return it->second;
}