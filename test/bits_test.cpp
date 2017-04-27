#include <vector>
#include <algorithm>
#include <sstream>

#include "gtest/gtest.h"

#include "soundbag/bits.hpp"

TEST( bits, enc_dec ){
	int16_t a = 0x0FFF;

	std::vector<byte> vl = soundbag::encodeBytes<int16_t>( a, true );
	ASSERT_EQ( vl.size(), 2 );
	ASSERT_EQ( vl[1], 0x0F );
	ASSERT_EQ( vl[0], 0xFF );

	std::vector<byte> vb = soundbag::encodeBytes<int16_t>( a, false );
	ASSERT_EQ( vb.size(), 2 );
	ASSERT_EQ( vb[0], 0x0F );
	ASSERT_EQ( vb[1], 0xFF );

	int16_t rl = soundbag::decodeBytes<int16_t>( vl, true );
	ASSERT_EQ( a, rl );

	int16_t rb = soundbag::decodeBytes<int16_t>( vb, false );
	ASSERT_EQ( a, rb );

	int32_t rl32 = soundbag::decodeBytes<int32_t>( vl, true );
	ASSERT_EQ( a, rl32 );

	int32_t rb32 = soundbag::decodeBytes<int32_t>( vb, false );
	ASSERT_EQ( a, rb32 );
}

TEST( bits, read_write ){
	int16_t a = 0x0FFF;

	for( int i = 0; i < 2; ++i ){
		bool little = ( i == 0 );
		std::ostringstream oss;
		soundbag::writeBytes<int16_t>( oss, a, little );
		ASSERT_FALSE( oss.fail() );
	
		std::istringstream iss( oss.str() );
		int16_t r = soundbag::readBytes<int16_t>( iss, little );
		ASSERT_FALSE( iss.fail() );
		ASSERT_EQ(  a, r );
	}
}