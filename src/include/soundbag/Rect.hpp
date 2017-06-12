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
	template<typename T>
	struct Rect {
		T left, top, right, bottom;
	
		Rect(){
			left = right = top = bottom = 0;
		}

		Rect( T _left, T _bottom, T _right, T _top ){
			left   = _left;
			top    = _top;
			right  = _right;
			bottom = _bottom;
		}

		Rect( const Rect& b ){
			left   = b.left;
			right  = b.right;
			top    = b.top;
			bottom = b.bottom;
		}

		template<typename T2>
		Rect( const Rect<T2>& b ){
			left   = (T)b.left;
			right  = (T)b.right;
			top    = (T)b.top;
			bottom = (T)b.bottom;
		}

		static inline Rect centered( const Point<T>& pos, const Point<T>& size ){
			return Rect( pos.x - size.x / 2, pos.y - size.y / 2, pos.x + size.x / 2, pos.y + size.y / 2 );
		}

		inline bool operator==( const Rect& b ) const {
			return ( left == b.left && top == b.top && right == b.right && bottom == b.bottom );
		}

		inline Rect operator+( const Point<T>& p ) const {
			return Rect( left + p.x, bottom + p.y, right + p.x, top + p.y );
		}

		inline Rect operator-( const Point<T>& p ) const {
			return (*this) + Point<T>( -p.x, -p.y );
		}

		inline bool hit( T x, T y ) const {
			return ( smaller( left, right ) <= x && x <= bigger( left, right ) && 
					 smaller( top, bottom ) <= y && y <= bigger( top, bottom ) );
		}

		inline bool hit( const PointF& p ) const {
			return hit( p.x, p.y );
		}
	};

	typedef Rect<float> RectF;
}

#endif