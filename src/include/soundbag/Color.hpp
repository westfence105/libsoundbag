#pragma once
/*
	libsoundbag

	Copyright (c) 2017 Togo Nishigaki

	This software released under MIT License
	http://opensource.org/licenses/mit-license.php
*/

#ifndef libsounbag_Color_h
#define libsounbag_Color_h

#include <cstdint>
#include <iostream>
#include <sstream>
#include <iomanip>

typedef uint8_t byte;

namespace soundbag {
	struct ColorF;
	struct ColorB;

	struct ColorF {
		float r,g,b,a;

		inline ColorF() : ColorF(0,0,0,0) {}
		inline ColorF( float _r, float _g, float _b, float _a = 1 ){
			r = _r;
			g = _g;
			b = _b;
			a = _a;
		}

		inline bool operator==( const ColorF& c ) const {
			return ( r == c.r ) && ( g == c.g ) && ( b == c.b ) && ( a == c.a );
		}
	};

	struct ColorB {
		byte r,g,b,a;

		inline ColorB() : ColorB(0,0,0,0){}
		inline ColorB( byte _r, byte _g, byte _b, byte _a = 0xFF ){
			r = _r;
			g = _g;
			b = _b;
			a = _a;
		}

		inline bool operator==( const ColorB& c ) const {
			return ( r == c.r ) && ( g == c.g ) && ( b == c.b ) && ( a == c.a );
		}

		inline ColorB( const ColorF& c ){
			r = (byte)( c.r * 0xFF );
			g = (byte)( c.g * 0xFF );
			b = (byte)( c.b * 0xFF );
			a = (byte)( c.a * 0xFF );
		}

		inline operator ColorF() const {
			return ColorF( (float) r /0xFF, (float) g / 0xFF, (float) b / 0xFF, (float) a / 0xFF );
		}

		operator std::string() const {
			std::ostringstream oss;
			oss << "#" << std::hex << std::setfill('0') << std::setw(2) 
			    << r << g << b << a;
			return oss.str();
		}

	#ifdef _SDL_H
		operator SDL_Color() const {
			SDL_Color ret;
			ret.r = r;
			ret.g = g;
			ret.b = b;
			ret.a = a;
			return ret;
		}
	#endif

	};

	inline std::ostream& operator<<( std::ostream& os, const ColorB& c ){
		os << "#" << std::hex << std::setfill('0') << std::setw(2)
				  << c.r << c.g << c.b << c.a;
		return os;
	}
}

#endif