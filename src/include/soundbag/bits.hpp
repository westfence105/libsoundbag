#pragma once
/*
	libsoundbag

	Copyright (c) 2017 Togo Nishigaki

	This software released under MIT License
	http://opensource.org/licenses/mit-license.php
*/

#ifndef libsoundbag_bits_h
#define libsoundbag_bits_h

#include <vector>
#include <algorithm>

#include "soundbag.hpp"

namespace soundbag {
	inline const bool isLittleEndian(){
		static const int i = 1;
		return (bool)*((byte*)&i);
	}
	
	template<typename T>
	std::vector<byte> encodeBytes( const T& data, bool little_endian = false ){
		const byte* p = reinterpret_cast<const byte*>(&data);
		std::vector<byte> ret( p, p + sizeof(T) );
		if( isLittleEndian() != little_endian ){
			std::reverse( ret.begin(), ret.end() );
		}
		return ret;
	}
	
	template<typename T>
	T decodeBytes( std::vector<byte> data, bool little_endian = false ){
		if( sizeof(T) > data.size() ){
			const size_t diff = sizeof(T) - data.size();
			auto iter = little_endian ? data.end() : data.begin();
			data.insert( iter, diff, (byte)0 );
		}
		if( isLittleEndian() != little_endian ){
			std::reverse( data.begin(), data.end() );
		}
		return *(reinterpret_cast<T*>(data.data()));
	}

	template<typename T>
	T decodeBytes( std::vector<byte>::const_iterator& iter, bool little_endian = false ){
		const auto before = iter;
		iter += sizeof(T);
		return decodeBytes<T>( std::vector<byte>( before, iter ), little_endian );
	}
	
	template<typename T>
	std::ostream& writeBytes( std::ostream& os, const T& data, bool little_endian = false ){
		return os.write( (const char*)encodeBytes<T>( data, little_endian ).data(), sizeof(T) );
	}
	
	template<typename T>
	T readBytes( std::istream& is, bool little_endian = false ){
		char buf[sizeof(T)];
		is.read( buf, sizeof(T) );
		if( isLittleEndian() == little_endian ){
			return *(reinterpret_cast<T*>(buf));
		}
		else {
			std::vector<byte> v ( buf, buf + sizeof(T) );
			std::reverse( v.begin(), v.end() );
			return *(reinterpret_cast<T*>(v.data()));
		}
	}
}

#endif