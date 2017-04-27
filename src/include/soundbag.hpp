#pragma once
/*
	libsoundbag

	Copyright (c) 2017 Togo Nishigaki

	This software released under MIT License
	http://opensource.org/licenses/mit-license.php
*/

#ifndef libsoundbag_soundbag_h
#define libsoundbag_soundbag_h

#include <cstdint>

typedef uint8_t byte;

namespace soundbag {
	template<typename T>
	inline T bigger( const T& a, const T& b ){
		return ( a > b ) ? a : b;
	}
	
	template<typename T>
	inline T smaller( const T& a, const T& b ){
		return ( a < b ) ? a : b;
	}

	int32_t xor128();
}

#endif