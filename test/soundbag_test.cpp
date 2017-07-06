#include "gtest/gtest.h"

#include "soundbag/soundbag.hpp"
#include "soundbag/Point.hpp"
#include "soundbag/Color.hpp"
#include "soundbag/Rect.hpp"

using namespace soundbag;

TEST( soundbag, xor128 ){
	std::vector<int32_t> v;
	v.reserve(10);

	for( int i = 0; i < 10; ++i ){
		v.push_back( xor128() );
	}

	for( size_t i = 0; i < v.size(); ++i ){
		for( size_t j = 0; j < v.size(); ++j ){
			if( i == j ){
				continue;
			}
			ASSERT_NE( v[i], v[j] );
		}
	}
}

TEST( soundbag, Point ){
	Point<float> p1( 0, 0, 0 );
	Point<float> p2( 0, 1, 0 );
	EXPECT_EQ( p1 + p2, Point<float>( 0,  1, 0 ) );
	EXPECT_EQ( p1 - p2, Point<float>( 0, -1, 0 ) );
	EXPECT_EQ( p2, Point<int>( 0, 1, 0 ) );
}

TEST( soundbag, Color ){
	ColorB  cb( 0x80, 0x80, 0x80 );
	ColorF cf = (ColorF)cb;
	EXPECT_EQ( cb, (ColorB)cf );
}

TEST( soundbag, Rect ){
	RectF r1 = RectF::centered( PointF( 5, 5 ), PointF( 10, 10 ) );
	EXPECT_EQ( r1, RectF( 0, 0, 10, 10 ) );
	EXPECT_EQ( r1 + PointF( 1, 2 ), RectF(  1,  2, 11, 12 ) );
	EXPECT_EQ( r1 - PointF( 1, 2 ), RectF( -1, -2,  9,  8 ) );
	EXPECT_TRUE( r1.hit( 8, 8 ) );
	EXPECT_TRUE( r1.hit( PointF( 8, 8 ) ) );
	EXPECT_FALSE( r1.hit( 12,  8 ) );
	EXPECT_FALSE( r1.hit(  8, 12 ) );
	EXPECT_FALSE( r1.hit( PointF( 12,  8 ) ) );
	EXPECT_FALSE( r1.hit( PointF(  8, 12 ) ) );
}