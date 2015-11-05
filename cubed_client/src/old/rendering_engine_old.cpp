#include "rendering_engine.h"
#define GLEW_STATIC
#include <glew/include/glew.h>
#include <glm/include/gtc/matrix_transform.hpp>
#include "input_manager.h"
#include "cubed_exception.h"

RenderingEngine::RenderingEngine(const std::string& wTitle, unsigned int wWidth, unsigned int wHeight) :
	m_isCloseRequested(false), m_pCamera(nullptr), m_aspectRatio(static_cast<float>(wWidth) / wHeight)
{
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
		throw CubedException(CE_SDL_INIT_FAILED);

	m_window = SDL_CreateWindow(wTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, wWidth, wHeight, SDL_WINDOW_OPENGL);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

	m_glContext = SDL_GL_CreateContext(m_window);

	//glewExperimental = GL_TRUE;
	GLenum status = glewInit();

	if(status != GLEW_OK)
		throw CubedException(CE_GLEW_INIT_FAILED);

	m_isCloseRequested = false;

	m_2DTransform = glm::ortho(0.0f, static_cast<float>(wWidth), 0.0f, static_cast<float>(wHeight), 0.0f, 1.0f);

	glClearColor(0.2f, 0.3f, 1.0f, 1.0f);
	glClearDepth(1.0f);

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

RenderingEngine::~RenderingEngine()
{
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);

	SDL_Quit();
}

void RenderingEngine::swapBuffers()
{
	SDL_GL_SwapWindow(m_window);
}

void RenderingEngine::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderingEngine::setTexture(const Texture& texture)
{
	glBindTexture(GL_TEXTURE_2D, texture.m_texture);
}

void RenderingEngine::getWindowSize(int* w, int* h)
{
	SDL_GetWindowSize(m_window, w, h);
}

int RenderingEngine::getWindowWidth()
{
	int w;
	getWindowSize(&w, nullptr);
	return w;
}

int RenderingEngine::getWindowHeight()
{
	int h;
	getWindowSize(nullptr, &h);
	return h;
}

void RenderingEngine::setMousePos(int x, int y)
{
	SDL_WarpMouseInWindow(m_window, x, y);
}