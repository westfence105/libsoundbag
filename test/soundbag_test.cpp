#include "gtest/gtest.h"

#include "soundbag.hpp"

TEST( soundbag, xor128 ){
	std::vector<int32_t> v;
	v.reserve(10);

	for( int i = 0; i < 10; ++i ){
		v.push_back( soundbag::xor128() );
	}

	for( int i = 0; i < v.size(); ++i ){
		for( int j = 0; j < v.size(); ++j ){
			if( i == j ){
				continue;
			}
			ASSERT_NE( v[i], v[j] );
		}
	}
}