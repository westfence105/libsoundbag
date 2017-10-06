#include "gtest/gtest.h"

#include <sstream>

#include "soundbag/utils.hpp"

using namespace soundbag;

TEST( utils, pow ){
	EXPECT_EQ( simple_pow( 2, 0 ), 1 );
	EXPECT_EQ( simple_pow( 0, 0 ), 1 );
	EXPECT_EQ( simple_pow( 0, 1 ), 0 );
	EXPECT_EQ( simple_pow( 0, 100 ), 0 );
	EXPECT_EQ( simple_pow( 3, 3 ), 27 );
	EXPECT_EQ( simple_pow( 2, 10 ), 1024 );

	EXPECT_EQ( p2( 2 ), 4 );
	EXPECT_EQ( p2( 12 ), 144 );

	EXPECT_EQ( p3( 2 ), 8 );
	EXPECT_EQ( p3( 3 ), 27 );
}

TEST( utils, swap ){
	int a = 1;
	int b = 2;
	swap( a, b );
	EXPECT_EQ( a, 2 );
	EXPECT_EQ( b, 1 );
}

TEST( utils, in_range ){
	EXPECT_TRUE( in_range( 1,  0, 2 ) );
	EXPECT_TRUE( in_range( -10, 0, -20 ) );
	EXPECT_TRUE( in_range( 128, 127, 256 ) );
	EXPECT_TRUE( in_range( 128, 256, 127 ) );
	EXPECT_FALSE( in_range( 1, 2, 3 ) );
	EXPECT_FALSE( in_range( -1, 0, 1024 ) );
}

TEST( utils, approx ){
	EXPECT_TRUE( approx( 0.102, 0.1 ) );
	EXPECT_TRUE( approx( 0.75, (double)3/4 ) );
	EXPECT_FALSE( approx( 0.1, 0.125 ) );
}

TEST( utils, compare_and_select ){
	EXPECT_EQ( bigger( 1, 12 ), 12 );
	EXPECT_EQ( bigger( 12, 1 ), 12 );
	EXPECT_EQ( bigger( -1, -8 ), -1 );
	EXPECT_EQ( bigger( -128, 127 ), 127 );

	EXPECT_EQ( smaller( 1, 12 ), 1 );
	EXPECT_EQ( smaller( 12, 1 ), 1 );
	EXPECT_EQ( smaller( -1, -8 ), -8 );
	EXPECT_EQ( smaller( -128, 127 ), -128 );
}

TEST( utils, is_numeric ){
	is_numeric is_decimal;
	EXPECT_TRUE( is_decimal('1') );
	EXPECT_TRUE( is_decimal('-') );
	EXPECT_TRUE( is_decimal('.') );
	EXPECT_TRUE( is_decimal('e') );
	EXPECT_TRUE( is_decimal("1") );
	EXPECT_TRUE( is_decimal("-1") );
	EXPECT_TRUE( is_decimal("1.5") );
	EXPECT_TRUE( is_decimal("-1.5") );
	EXPECT_TRUE( is_decimal("6.0e-23") );
	EXPECT_FALSE( is_decimal("a") );
	EXPECT_FALSE( is_decimal("hoge") );

	is_numeric is_integer( 10, is_numeric::NUMERIC_TYPE_SIGNED_INT );
	EXPECT_TRUE( is_integer('1') );
	EXPECT_TRUE( is_integer('-') );
	EXPECT_FALSE( is_integer('.') );
	EXPECT_FALSE( is_integer('e') );
	EXPECT_TRUE( is_integer("1") );
	EXPECT_TRUE( is_integer("-1") );
	EXPECT_FALSE( is_integer("1.5") );
	EXPECT_FALSE( is_integer("6.0e-23") );
	EXPECT_FALSE( is_integer("a") );
	EXPECT_FALSE( is_integer("hoge") );

	is_numeric is_uint( 10, is_numeric::NUMERIC_TYPE_UNSIGNED_INT );
	EXPECT_TRUE( is_uint('1') );
	EXPECT_FALSE( is_uint('-') );
	EXPECT_FALSE( is_uint('.') );
	EXPECT_FALSE( is_uint('e') );
	EXPECT_TRUE( is_uint("1") );
	EXPECT_FALSE( is_uint("-1") );
	EXPECT_FALSE( is_uint("1.5") );
	EXPECT_FALSE( is_uint("6.0e-23") );
	EXPECT_FALSE( is_uint("a") );
	EXPECT_FALSE( is_uint("hoge") );

	is_numeric is_octal( 8, is_numeric::NUMERIC_TYPE_UNSIGNED_INT );
	EXPECT_TRUE( is_octal('1') );
	EXPECT_TRUE( is_octal('7') );
	EXPECT_FALSE( is_octal('8') );
	EXPECT_FALSE( is_octal('-') );
	EXPECT_FALSE( is_octal('.') );
	EXPECT_FALSE( is_octal('e') );
	EXPECT_TRUE( is_octal("1") );
	EXPECT_TRUE( is_octal("01") );
	EXPECT_TRUE( is_octal("7") );
	EXPECT_FALSE( is_octal("8") );
	EXPECT_FALSE( is_octal("-1") );
	EXPECT_FALSE( is_octal("1.5") );
	EXPECT_FALSE( is_octal("6.0e-23") );
	EXPECT_FALSE( is_octal("a") );
	EXPECT_FALSE( is_octal("hoge") );

	is_numeric is_hex( 16, is_numeric::NUMERIC_TYPE_UNSIGNED_INT );
	EXPECT_TRUE( is_hex('1') );
	EXPECT_TRUE( is_hex('A') );
	EXPECT_TRUE( is_hex('F') );
	EXPECT_TRUE( is_hex('a') );
	EXPECT_TRUE( is_hex('f') );
	EXPECT_FALSE( is_hex('g') );
	EXPECT_FALSE( is_hex('-') );
	EXPECT_FALSE( is_hex('.') );
	EXPECT_TRUE( is_hex("1") );
	EXPECT_TRUE( is_hex("a") );
	EXPECT_TRUE( is_hex("A") );
	EXPECT_TRUE( is_hex("f") );
	EXPECT_TRUE( is_hex("0x01") );
	EXPECT_FALSE( is_hex("g") );
	EXPECT_FALSE( is_hex("-1") );
	EXPECT_FALSE( is_hex("1.5") );
	EXPECT_FALSE( is_hex("6.0e-23") );
	EXPECT_FALSE( is_hex("hoge") );
}

TEST( utils, match_any_of ){
	EXPECT_TRUE((  match_any_of<int,1,8,16>( 1 )  ));
	EXPECT_TRUE((  match_any_of<int,1,8,16>( 8 )  ));
	EXPECT_TRUE((  match_any_of<int,1,8,16>( 16 ) ));
	EXPECT_FALSE(( match_any_of<int,1,8,16>( 2 )  ));
	EXPECT_FALSE(( match_any_of<int,1,8,16>( 4 )  ));
	EXPECT_FALSE(( match_any_of<int,1,8,16>( 15 ) ));
}

TEST( utils, ordinal ){
	std::ostringstream oss;
	oss << ordinal(1);
	EXPECT_EQ( oss.str(), "1st" );

	oss.str("");
	oss << ordinal(2);
	EXPECT_EQ( oss.str(), "2nd" );

	oss.str("");
	oss << ordinal(3);
	EXPECT_EQ( oss.str(), "3rd" );

	oss.str("");
	oss << ordinal(4);
	EXPECT_EQ( oss.str(), "4th" );

	oss.str("");
	oss << ordinal(8);
	EXPECT_EQ( oss.str(), "8th" );

	oss.str("");
	oss << ordinal(11);
	EXPECT_EQ( oss.str(), "11th" );

	oss.str("");
	oss << ordinal(12);
	EXPECT_EQ( oss.str(), "12th" );

	oss.str("");
	oss << ordinal(21);
	EXPECT_EQ( oss.str(), "21st" );

	oss.str("");
	oss << ordinal(52);
	EXPECT_EQ( oss.str(), "52nd" );

	oss.str("");
	oss << ordinal(43);
	EXPECT_EQ( oss.str(), "43rd" );

	oss.str("");
	oss << ordinal(34);
	EXPECT_EQ( oss.str(), "34th" );
}