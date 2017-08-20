#pragma once
/*
	libsoundbag

	Copyright (c) 2017 Togo Nishigaki

	This software released under MIT License
	http://opensource.org/licenses/mit-license.php
*/

#ifndef libsoundbag_SBGL_Object_h
#define libsoundbag_SBGL_Object_h

#include <vector>
#include <memory>

#include <GL/gl.h>

#include "soundbag.hpp"
#include "Bitmap.hpp"

namespace soundbag {
	class SBGL_Object {
		GLuint vbo[2];
		GLuint tex;

		std::vector<GLfloat> vdata;
		std::vector<GLuint>  idata;

		int coord_size;
		int color_size;
		int texcoord_size;

		std::shared_ptr<Bitmap> tex_image;

	public:
		SBGL_Object();
		SBGL_Object( const std::vector<GLfloat>& vd, int _coord_size, int _color_size, int _texcoord_size );
		virtual ~SBGL_Object();

		void setVertex( const std::vector<GLfloat>& vd, int _coord_size, int _color_size, int _texcoord_size );
		void setIndex( const std::vector<GLuint>& id );
		void setTexImage( std::shared_ptr<Bitmap> img );

		virtual void genBuffers();

	};
}

#endif