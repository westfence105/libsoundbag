/*
	libsoundbag

	Copyright (c) 2017 Togo Nishigaki

	This software released under MIT License
	http://opensource.org/licenses/mit-license.php
*/
#include <GL/gl.h>
#include "SDL.h"

#include "soundbag/SDL_GL_Window.hpp"

using namespace soundbag;

SDL_GL_Window::SDL_GL_Window( const char* title, const SDL_GL_Window::Config* conf ) throw( std::runtime_error )
	: frame_rate( conf->frame_rate )
{
	uint32_t flags = SDL_WINDOW_OPENGL;

	if( conf->window_mode == Config::WINDOW_MODE_FULLSCREEN ){
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
	else if( conf->window_mode == Config::WINDOW_MODE_MAXIMIZED ){
		flags |= SDL_WINDOW_MAXIMIZED;
	}
	else if( conf->window_mode == Config::WINDOW_MODE_MINIMIZED ){
		flags |= SDL_WINDOW_MINIMIZED;
	}

	window = SDL_CreateWindow( title, conf->x, conf->y, conf->width, conf->height, flags );
	if( window == NULL ){
		throw std::runtime_error("Failed to create window.");
	}

	context = SDL_GL_CreateContext(window);
	if( context == NULL ){
		throw std::runtime_error("Failed to create OpenGL context.");
	}

	onResize( conf->width, conf->height );

	setBackground( conf->background );

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
}

SDL_GL_Window::~SDL_GL_Window(){
	SDL_DestroyWindow(window);
}

void SDL_GL_Window::onResize( int w, int h ){
	width  = w;
	height = h;
	glViewport( 0, 0, w, h );
	setProjection();
}

void SDL_GL_Window::setProjection(){
	const double w = getWidth();
	const double h = getHeight();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho( -w/2, w/2, -h/2, h/2, -1.0, 1.0 );
}

void SDL_GL_Window::prepare(){

}

void SDL_GL_Window::main(){
	prepare();

	SDL_Event event;
	const uint32_t interval = 1000 / frame_rate;
	uint32_t tick = SDL_GetTicks();
	uint32_t next_frame = tick;
	uint32_t last_frame = tick;
	quit = false;
	while(!quit){
		while( SDL_PollEvent(&event) ){
			if( handleEvent(event) ){
				continue;
			}
			else if( SDL_QUIT ){
				return;
			}
			else if( event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED ){
				onResize( event.window.data1, event.window.data2 );
			}
		}
		if( quit ){ break; }

		tick = SDL_GetTicks();
		if( next_frame <= tick ){
			update( tick - last_frame ); 
			draw();
			SDL_GL_SwapWindow(window);
			last_frame = tick;
			next_frame = tick + interval;
		}
	}
}

bool SDL_GL_Window::handleEvent( const SDL_Event& event ){
	if( event.key.keysym.sym == SDLK_q || event.key.keysym.sym == SDLK_ESCAPE ){
		quit = true;
		return true;
	}
}

int SDL_GL_Window::getWidth() const {
	return width;
}

int SDL_GL_Window::getHeight() const {
	return height;
}

void SDL_GL_Window::setBackground( const ColorF& c ){
	background = c;
	glClearColor( c.r, c.g, c.b, c.a );
}

void SDL_GL_Window::resize( int w, int h ){
	SDL_SetWindowSize( window, w, h );
	onResize( w, h );
}