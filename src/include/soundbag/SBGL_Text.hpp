#pragma once
/*
	libsoundbag

	Copyright (c) 2017 Togo Nishigaki <westfence95@gmail.com>

	This software released under MIT License
	http://opensource.org/licenses/mit-license.php


	Depends:

	Simple DirectMedia Layer
	Copyright (C) 1997-2016 Sam Lantinga <slouken@libsdl.org>

	SDL_ttf
	Copyright (C) 2001-2016 Sam Lantinga <slouken@libsdl.org>

	The OpenGL Extension Wrangler Library
	Copyright (C) 2008-2015, Nigel Stewart <nigels[]users sourceforge net>
	Copyright (C) 2002-2008, Milan Ikits <milan ikits[]ieee org>
	Copyright (C) 2002-2008, Marcelo E. Magallon <mmagallo[]debian org>
	Copyright (C) 2002, Lev Povalahev
	All rights reserved.
*/

#ifndef libsoundbag_SBGL_Text_h
#define libsoundbag_SBGL_Text_h

#include <exception>
#include <string>
#include <array>

#include <GL/gl.h>
#include "SDL.h"
#include "SDL_ttf.h"

#include "Color.hpp"

namespace soundbag {
	class SBGL_Text {
		SDL_Surface* img;

		GLuint tex;
		GLuint vbo[2];

		ColorF m_color;

		int m_v_align;
		int m_h_align;

		float width, height;

		std::array<GLfloat,4*4> data_v;
		std::array<GLuint,6> index_v;

	public:
		static const int SBGL_TEXT_ALIGN_CENTER;
		static const int SBGL_TEXT_ALIGN_LEFT;
		static const int SBGL_TEXT_ALIGN_RIGHT;
		static const int SBGL_TEXT_ALIGN_TOP;
		static const int SBGL_TEXT_ALIGN_BOTTOM;

		SBGL_Text( const std::string& str, TTF_Font* font, const ColorF& color ) 
			throw(std::invalid_argument,std::runtime_error);
		virtual ~SBGL_Text();

		void genBuffers();
		void draw() const;

		void setAlignment( int vertical, int horizontal );
		void setColor( const ColorF& color );

	private:
		SBGL_Text(const SBGL_Text&);

	};
}

#endif