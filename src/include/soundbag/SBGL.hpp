#pragma once
/*
	libsoundbag

	Copyright (c) 2017 Togo Nishigaki

	This software released under MIT License
	http://opensource.org/licenses/mit-license.php
*/

#ifndef libsoundbag_SBGL_h
#define libsoundbag_SBGL_h

#include <vector>

#include <GL/gl.h>

#include "Point.hpp"
#include "Color.hpp"
#include "Rect.hpp"

namespace soundbag {
	inline void push_coord( std::vector<GLfloat>& v, const PointF& pos, const ColorF& color ){
		v.push_back(pos.x);
		v.push_back(pos.y);
		v.push_back(pos.z);
	
		v.push_back(color.r);
		v.push_back(color.g);
		v.push_back(color.b);
		v.push_back(color.a);
	}

	inline void push_rect( std::vector<GLfloat>& v, const RectF& rect, float z, const ColorF& color ){
		push_coord( v, PointF( rect.left,  rect.top,    z ), color );
		push_coord( v, PointF( rect.right, rect.top,    z ), color );
		push_coord( v, PointF( rect.right, rect.bottom, z ), color );
		push_coord( v, PointF( rect.left,  rect.bottom, z ), color );
	}

	inline int nextPow2( int n ){
		if( n < 0 ){
			return 0;
		}
		if( n == 0 ){
			return 1;
		}

		return (int)round( pow( 2, floor( log2( n - 1 ) ) + 1 ));
	}
}

#endif