/*
	libsoundbag

	Copyright (c) 2017 Togo Nishigaki <westfence95@gmail.com>

	This software released under MIT License
	http://opensource.org/licenses/mit-license.php
*/
/*
	Depends:

	The OpenGL Extension Wrangler Library
	Copyright (C) 2008-2015, Nigel Stewart <nigels[]users sourceforge net>
	Copyright (C) 2002-2008, Milan Ikits <milan ikits[]ieee org>
	Copyright (C) 2002-2008, Marcelo E. Magallon <mmagallo[]debian org>
	Copyright (C) 2002, Lev Povalahev
	All rights reserved.
*/

#include <vector>
#include <numeric>
#include <memory>
#include <cmath>

#include <GL/glew.h>

#include "soundbag/SBGL_Object.hpp"

using namespace soundbag;

SBGL_Object::SBGL_Object(){
	vbo[0] = vbo[1] = tex = 0;
	coord_size = 3;
	color_size = 4;
	texcoord_size = 2;
}

SBGL_Object::~SBGL_Object(){
	glDeleteBuffers( 2, vbo );
	glDeleteTextures( 1, &tex );
}

SBGL_Object::SBGL_Object( const std::vector<GLfloat>& vd, int _coord_size, int _color_size, int _texcoord_size ){
	setVertex( vd, _coord_size, _color_size, _texcoord_size );
	idata.resize( (unsigned int)floor( vd.size() / (double)( _coord_size + color_size + _texcoord_size ) ) );
	std::iota( idata.begin(), idata.end(), 0 );
}

void SBGL_Object::setVertex( const std::vector<GLfloat>& vd, int _coord_size, int _color_size, int _texcoord_size ){
	std::vector<GLfloat>( vd.begin(), vd.end() ).swap( vdata );
}

void SBGL_Object::setIndex( const std::vector<GLuint>& id ){
	std::vector<GLuint>( id.begin(), id.end() ).swap( idata );
}

void SBGL_Object::setTexImage( std::shared_ptr<Bitmap> img ){
	tex_image = img;
}

void SBGL_Object::genBuffers(){
	glGenBuffers( 2, vbo );

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0] );
	glBufferData(GL_ARRAY_BUFFER, vdata.size()*sizeof(GLfloat), vdata.data(), GL_STATIC_DRAW );
	glBindBuffer(GL_ARRAY_BUFFER, 0 );

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1] );
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, idata.size()*sizeof(GLfloat), idata.data(), GL_STATIC_DRAW );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0 );

	if( tex_image ){
		GLint tex_format = GL_RGB;
		if( tex_image->format == Bitmap::BitmapFormat::RGBA ){
			tex_format = GL_RGBA;
		}

		glGenTextures( 1, &tex );
		glBindTexture(GL_TEXTURE_2D, tex );
		gluBuild2DMipmaps(GL_TEXTURE_2D, tex_format, tex_image->width, tex_image->height, 
							tex_format, GL_UNSIGNED_BYTE, tex_image->data.data() );

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
}