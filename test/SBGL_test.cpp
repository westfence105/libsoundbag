#include "gtest/gtest.h"

#include <memory>
#include <deque>

#include <GL/glew.h>

#include "soundbag/SDL_GL_Window.hpp"
#include "soundbag/SBGL.hpp"
#include "soundbag/SBGL_Object.hpp"
#include "soundbag/SBGL_Dialog.hpp"
#include "soundbag/SBGL_Text.hpp"

using namespace soundbag;

class SBGL_TestWindow : SDL_GL_Window {
	std::deque<std::string>& errors;

	std::shared_ptr<SBGL_Object> test_obj;

public:
	SBGL_TestWindow( std::deque<std::string>& errors );
	~SBGL_TestWindow();

	void prepare();
	void draw();

};

int main( int argc, char** argv ){
	testing::InitGoogleTest( &argc, argv );

	return RUN_ALL_TESTS();
}

TEST( SBGL, push_coord ){
	std::vector<GLfloat> v;
	push_coord( v, PointF( 0, 1, 2 ), ColorF( 1, 0.5, 0 ) );
	std::vector<GLfloat> v2 = {
		0, 1, 2,  1, 0.5, 0, 1
	};
	EXPECT_EQ( v, v2 );
}

TEST( SBGL, push_rect ){
	std::vector<GLfloat> v;
	push_rect( v, RectF( 4, 2, 8, 6 ), 1, ColorF( 1, 0.5, 0 ) );
	std::vector<GLfloat> v2 = {
		4, 6, 1,  1, 0.5, 0, 1,
		8, 6, 1,  1, 0.5, 0, 1,
		8, 2, 1,  1, 0.5, 0, 1,
		4, 2, 1,  1, 0.5, 0, 1,
	};
	EXPECT_EQ( v, v2 );
}

TEST( SBGL, nextPow2 ){
	EXPECT_EQ( nextPow2( -1 ),  0 );
	EXPECT_EQ( nextPow2(  0 ),  1 );
	EXPECT_EQ( nextPow2(  1 ),  1 );
	EXPECT_EQ( nextPow2(  2 ),  2 );
	EXPECT_EQ( nextPow2(  3 ),  4 );
	EXPECT_EQ( nextPow2( 12 ), 16 );
}

TEST( SBGL, drawing ){
	std::deque<std::string> errors;

	auto window = std::make_shared<SBGL_TestWindow>()

	auto ret = glewInit();
	if( ret != GLEW_OK ){
		std::cerr << "GLEW Error: " << glewGetErrorString(ret) << std::endl;
		return 1;
	}

	window->main();
}

TEST( SBGL, SBGL_Object_instance ){
	auto obj = std::make_shared<SBGL_Object>();
}