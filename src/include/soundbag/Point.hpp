#pragma once
/*
	libsoundbag

	Copyright (c) 2017 Togo Nishigaki

	This software released under MIT License
	http://opensource.org/licenses/mit-license.php
*/

#ifndef libsoundbag_Point_h
#define libsoundbag_Point_h

#include <cstdint>

namespace soundbag {
	template<typename T>
	struct Point {
		T x,y,z;

		inline Point() : Point(0,0,0) {}
		inline Point( T _x, T _y, T _z = 0 ){
			x = _x;
			y = _y;
			z = _z;
		}

		inline Point( const Point& b ){
			x = b.x;
			y = b.y;
			z = b.z;
		}

		template<typename T2>
		inline Point( const Point<T2>& b ){
			x = (T)b.x;
			y = (T)b.y;
			z = (T)b.z;
		}

		inline Point<T> operator+( const Point<T>& b ) const {
			return Point<T>( x + b.x, y + b.y, z + b.z );
		}

		inline Point<T> operator-( const Point<T>& b ) const {
			return Point<T>( x - b.x, y - b.y, z - b.z );
		}

		inline bool operator==( const Point<T>& b ) const {
			return ( x == b.x && y == b.y && z == b.z );
		}
	};

	typedef Point<float> PointF;
}

#endif