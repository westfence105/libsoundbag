#pragma once
/*
	libsoundbag

	Copyright (c) 2017 Togo Nishigaki

	This software released under MIT License
	http://opensource.org/licenses/mit-license.php
*/

#ifndef libsoundbag_Rect_h
#define libsoundbag_Rect_h

#include "soundbag.hpp"
#include "Point.hpp"

namespace soundbag {
	struct RectF {
		float left, top, right, bottom;
	
		RectF(){
			left = right = top = bottom = 0;
		}

		RectF( float _left, float _top, float _right, float _bottom ){
			left   = _left;
			top    = _top;
			right  = _right;
			bottom = _bottom;
		}

		static inline RectF centered( const PointF& pos, const PointF& size ){
			return RectF( pos.x - size.x / 2, pos.y + size.y / 2, pos.x + size.x / 2, pos.y - size.y / 2 );
		}

		bool hit( float x, float y ) const {
			return ( smaller( left, right ) <= x && x <= bigger( left, right ) && 
					 smaller( top, bottom ) <= y && y <= bigger( top, bottom ) );
		}

		bool hit( const PointF& p ) const {
			return hit( p.x, p.y );
		}
	};
}

#endif