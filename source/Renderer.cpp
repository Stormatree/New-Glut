#include "Renderer.hpp"

#include <stdio.h>

Renderer& Renderer::_instance(){
	static Renderer instance;
	return instance;
}

Renderer::~Renderer(){
	_running = false;

	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_sdlRenderer);
	SDL_GL_DeleteContext(_glcontext);
}

bool Renderer::_setupSDL(){
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0){
		printf("%s! %s: %s\n", "Failed to initialize SDL", "SDL Error", SDL_GetError());
		return false;
	}

	// Setting OpenGL to 2.1
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Renderer object
	_window = SDL_CreateWindow(_windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _windowSize.x(), _windowSize.y(), SDL_WINDOW_OPENGL);

	if (!_window){
		printf("%s! %s: %s\n", "Failed to create renderer", "SDL Error", SDL_GetError());
		return false;
	}

	// Renderer object (for 2D graphics only)
	_sdlRenderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);

	if (!_sdlRenderer){
		printf("%s! %s: %s\n", "Failed to create SDL renderer", "SDL Error", SDL_GetError());
		return false;
	}

	// Set size mode
	SDL_SetWindowFullscreen(_window, _windowMode);

	// Lock the mouse
	SDL_SetRelativeMouseMode(SDL_TRUE);

	return true;
}

bool Renderer::_setupGL(){
	// OpenGl context object
	_glcontext = SDL_GL_CreateContext(_window);

	if (!_glcontext){
		printf("%s! %s: %s\n", "Failed to create OpenGL context", "SDL Error", SDL_GetError());
		return false;
	}

	// OpenGl settings
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_CULL_FACE);

	// Extensions init
	GLenum error = glewInit();

	if (error != GLEW_OK){
		printf("%s! %s: %s\n", "Failed to initiate Glew", "Glew Error", glewGetErrorString(error));
		return false;
	}

	return true;
}

bool Renderer::reshape(){
	// Setting up OpenGL matrices
	glViewport(0, 0, _instance()._windowSize.x(), _instance()._windowSize.y());

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float ar = (float)_instance()._windowSize.x() / (float)_instance()._windowSize.y();

	gluPerspective(59, ar, 0.1, 1000); // 59 vfov = ~90 hfov

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLenum error = glGetError();

	if (error != GL_NO_ERROR){
		printf("%s! %s: %s\n", "Failed to reshape OpenGL matrices", "OpenGL Error", gluErrorString(error));
		return false;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return true;
}

bool Renderer::initiate(){
	// If already running, reset
	if (_instance()._running){
		SDL_DestroyWindow(_instance()._window);
		SDL_DestroyRenderer(_instance()._sdlRenderer);
		SDL_GL_DeleteContext(_instance()._glcontext);
	}
	else{
		_instance()._running = true;
	}

	// Returns false if failed to setup
	if (!_instance()._setupSDL())
		return false;

	if (!_instance()._setupGL())
		return false;

	return reshape();
}

void Renderer::swapBuffer(){
	// Swap and clean
	SDL_GL_SwapWindow(_instance()._window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

SDL_Renderer* Renderer::sdlRenderer(){
	return _instance()._sdlRenderer;
}

int Renderer::windowWidth(){
	return _instance()._windowSize.x();
}

int Renderer::windowHeight(){
	return _instance()._windowSize.y();
}

void Renderer::setWindowMode(WindowModes mode){
	_instance()._windowMode = mode;
	
	if (_instance()._running)
		SDL_SetWindowFullscreen(_instance()._window, mode);
}

void Renderer::setWindowSize(int width, int height){
	_instance()._windowSize = Vector2i(width, height);

	if (_instance()._running){
		SDL_SetWindowSize(_instance()._window, width, height);
		_instance().initiate();
	}
}

void Renderer::setWindowTitle(const char* title){
	_instance()._windowTitle = title;

	if (_instance()._running)
		SDL_SetWindowTitle(_instance()._window, title);
}