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

class SBGL_TestWindow : public SDL_GL_Window {
	std::deque<std::string>& errors;

	std::shared_ptr<SBGL_Object> test_obj;

public:
	SBGL_TestWindow( std::deque<std::string>& _errors );
	~SBGL_TestWindow();

	void prepare();
	void draw();
	void update( uint32_t delta );

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

	auto window = std::make_shared<SBGL_TestWindow>( errors );

	auto ret = glewInit();
	if( ret != GLEW_OK ){
		FAIL() << "GLEW Error: " << glewGetErrorString(ret) << std::endl;
	}

	window->main();

	if( 0 < errors.size() ){
		std::ostringstream oss;
		for( const std::string& err : errors ){
			oss << err << "\n";
		}
		FAIL() << oss.str();
	}
}

SBGL_TestWindow::SBGL_TestWindow( std::deque<std::string>& _errors ) 
	: SDL_GL_Window( "SBGL_Test", SDL_GL_Window::Config( 800, 600 ) ), 
		errors(_errors )
{
	test_obj = std::make_shared<SBGL_Object>();
}

SBGL_TestWindow::~SBGL_TestWindow(){

}

void SBGL_TestWindow::prepare(){
	test_obj->genBuffers();
}

void SBGL_TestWindow::draw(){

}

void SBGL_TestWindow::update( uint32_t delta ){
	static uint32_t time = 0;
	time += delta;
	if( 30 * 1000 < time ){
		quit();
	}
}