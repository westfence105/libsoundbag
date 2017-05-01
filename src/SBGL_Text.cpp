/*
	libsoundbag

	Copyright (c) 2017 Togo Nishigaki <westfence95@gmail.com>

	This software released under MIT License
	http://opensource.org/licenses/mit-license.php
*/
/*
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

#include <array>
#include <exception>

#include <GL/glew.h>
#include "SDL.h"
#include "SDL_ttf.h"

#include "soundbag/Color.hpp"
#include "soundbag/SBGL.hpp"
#include "soundbag/SBGL_Text.hpp"

namespace soundbag {
	const int SBGL_Text::SBGL_TEXT_ALIGN_CENTER =  0;
	const int SBGL_Text::SBGL_TEXT_ALIGN_LEFT	= -1;
	const int SBGL_Text::SBGL_TEXT_ALIGN_RIGHT	=  1;
	const int SBGL_Text::SBGL_TEXT_ALIGN_TOP	= -1;
	const int SBGL_Text::SBGL_TEXT_ALIGN_BOTTOM =  1;

	SBGL_Text::SBGL_Text( const std::string& str, TTF_Font* font, const ColorF& color ) 
		throw( std::invalid_argument, std::runtime_error )
	{
		m_color = color;
		m_v_align = SBGL_TEXT_ALIGN_CENTER;
		m_h_align = SBGL_TEXT_ALIGN_CENTER;
		tex = 0;
		vbo[0] = 0;
		vbo[1] = 0;

		if( font == NULL ){
			throw std::invalid_argument("No fonts was set.");
		}

		img = TTF_RenderUTF8_Blended( font, str.c_str(), (SDL_Color)ColorB( 0xFF, 0xFF, 0xFF ) );

		if( img->w * img->h == 0 ){
			throw std::runtime_error("Failed to create text texture.");
		}

		width  = img->w;
		height = img->h;

		data_v = {
			-width/2,  height/2,   0,  0,
			 width/2,  height/2,   1,  0,
			 width/2, -height/2,   1,  1,
			-width/2, -height/2,   0,  1,
		};

		index_v = {
			0, 1, 2,
			0, 2, 3,
		};
	}

	SBGL_Text::~SBGL_Text(){
		glDeleteBuffers( 2, vbo );
		glDeleteTextures( 1, &tex );
	}

	void SBGL_Text::genBuffers(){
		glGenBuffers( 2, vbo );

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0] );
		glBufferData(GL_ARRAY_BUFFER, data_v.size()*sizeof(GLfloat), data_v.data(), GL_STATIC_DRAW );
		glBindBuffer(GL_ARRAY_BUFFER, 0 );

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1] );
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_v.size()*sizeof(GLfloat), index_v.data(), GL_STATIC_DRAW );
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0 );

		glGenTextures( 1, &tex );
		glBindTexture(GL_TEXTURE_2D, tex );
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img->w, img->h, GL_RGBA, GL_UNSIGNED_BYTE, img->pixels );

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_PRIMARY_COLOR);
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_TEXTURE);
		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_PRIMARY_COLOR);
		glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA, GL_TEXTURE);
		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
	}

	void SBGL_Text::draw() const {
		if( vbo[0] * vbo[1] == 0 ){
			return;
		}

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		if( m_v_align != SBGL_TEXT_ALIGN_CENTER ){
			if( m_v_align == SBGL_TEXT_ALIGN_TOP ){
				glTranslatef( 0, -height/2, 0 );
			}
			else {
				glTranslatef( 0,  height/2, 0 );
			}
		}
		if( m_h_align != SBGL_TEXT_ALIGN_CENTER ){
			if( m_h_align == SBGL_TEXT_ALIGN_LEFT ){
				glTranslatef(  width/2, 0, 0 );
			}
			else {
				glTranslatef( -width/2, 0, 0 );
			}
		}

		glColor4f( m_color.r, m_color.g, m_color.b, m_color.a );

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, tex );
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0] );
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1] );
		glVertexPointer( 2, GL_FLOAT, sizeof(GLfloat) * 4, 0 );
		glTexCoordPointer( 2, GL_FLOAT, sizeof(GLfloat) * 4, (void*)( sizeof(GLfloat)*2 ) );
		glDrawElements(GL_TRIANGLES, index_v.size(), GL_UNSIGNED_INT, 0 );

		glBindBuffer(GL_ARRAY_BUFFER, 0 );
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0 );
		glBindTexture(GL_TEXTURE_2D, 0 );
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}

	void SBGL_Text::setAlignment( int vertical, int horizontal ){
		m_v_align = vertical;
		m_h_align = horizontal;
	}

	void SBGL_Text::setColor( const ColorF& c ){
		m_color = c;
	}
}