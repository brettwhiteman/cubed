#ifndef _RENDERINGENGINE_H_
#define _RENDERINGENGINE_H_

#include <string>
#include "texture.h"
#include "camera.h"
#include <sdl2/include/SDL.h>
#include "transform.h"

class RenderingEngine
{
public:
	RenderingEngine(const std::string& wTitle, unsigned int wWidth, unsigned int wHeight);
	~RenderingEngine();

	void swapBuffers();
	void clear();
	void setTexture(const Texture& texture);
	void getWindowSize(int* w, int* h);
	int getWindowWidth();
	int getWindowHeight();
	void setMousePos(int x, int y);
	void disableDepthTest() { glDisable(GL_DEPTH_TEST); }
	void enableDepthTest() { glEnable(GL_DEPTH_TEST); }

	
	bool m_isCloseRequested;
	float m_aspectRatio;

	Camera* m_pCamera;
	glm::mat4 m_2DTransform;

private:
	SDL_Window* m_window;
	SDL_GLContext m_glContext;
};

#endif