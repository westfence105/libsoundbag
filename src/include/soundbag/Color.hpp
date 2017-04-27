#pragma once
/*
	libsoundbag

	Copyright (c) 2017 Togo Nishigaki

	This software released under MIT License
	http://opensource.org/licenses/mit-license.php
*/

#ifndef __sb_Color_h__
#define __sb_Color_h__

#include <cstdint>

namespace soundbag {
	template<typename T, int MAX = 0xff>
	struct Color {
		T r,g,b,a;

		inline Color() : Color(0,0,0,0) {}
		inline Color( T _r, T _g, T _b, T _a = MAX ){
			r = _r;
			g = _g;
			b = _b;
			a = _a;
		}
	};

	typedef Color<uint8_t,0xff> ColorB;
	typedef Color<float,1> ColorF;
}

#endif