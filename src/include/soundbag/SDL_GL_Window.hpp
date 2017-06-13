#pragma once
/*
	libsoundbag

	Copyright (c) 2017 Togo Nishigaki

	This software released under MIT License
	http://opensource.org/licenses/mit-license.php
*/

#ifndef libsoundbag_SDL_GL_Window_h
#define libsoundbag_SDL_GL_Window_h

#include <stdexcept>
#include <mutex>

#include "SDL.h"
#include "Color.hpp"

namespace soundbag {

	class SDL_GL_Window {
	public:
		struct Config{
			static constexpr int WINDOW_MODE_NORMAL 	=  0;
			static constexpr int WINDOW_MODE_MAXIMIZED 	=  1;
			static constexpr int WINDOW_MODE_FULLSCREEN =  2;
			static constexpr int WINDOW_MODE_MINIMIZED	= -1;

			int width;
			int height;
			int x;
			int y;
			int window_mode;
			uint32_t frame_rate;
			ColorF background;
	
			inline Config( int w, int h ){
				width  = w;
				height = h; 
				x = SDL_WINDOWPOS_CENTERED;
				y = SDL_WINDOWPOS_CENTERED;
				window_mode = WINDOW_MODE_NORMAL;
				frame_rate = 60;
				background = ColorF(0,0,0,0);
			}
		};

	private:
		SDL_Window* m_window;
		SDL_GLContext m_context;
		bool is_fullscreen;
		int m_width;
		int m_height;
		ColorF m_background;
		uint32_t frame_rate;
		bool m_quit;
		mutable std::recursive_mutex m_mutex;

	protected:
		void onResize( int w, int h );

		virtual bool handleEvent( const SDL_Event& );
		virtual void setProjection();
		virtual void prepare();
		virtual void draw() = 0;
		virtual void update( uint32_t delta ) = 0;

	public:
		SDL_GL_Window( const std::string& title, const Config& conf ) throw(std::runtime_error);
		virtual ~SDL_GL_Window();

		void main();

		int width() const;
		int height() const;
		void resize( int w, int h );
		void setBackground( const ColorF& c );
		inline void setBackground( float r, float g, float b, float a = 1 ){ setBackground( ColorF(r,g,b,a) ); }
		void setFrameRate( uint32_t value );
		void quit();
	};
}

#endif