/*
	libsoundbag

	Copyright (c) 2017 Togo Nishigaki

	This software released under MIT License
	http://opensource.org/licenses/mit-license.php
*/
#include <exception>
#include <memory>

#include <GL/glew.h>

#include "SDL.h"
#include "SDL_ttf.h"

#include "soundbag/soundbag.hpp"
#include "soundbag/Color.hpp"
#include "soundbag/SBGL.hpp"
#include "soundbag/SBGL_Text.hpp"

#include "soundbag/SBGL_Dialog.hpp"

using namespace soundbag;

const int SBGL_Dialog::RESULT_ACCEPTED = 1;
const int SBGL_Dialog::RESULT_REJECTED = 0;
const int SBGL_Dialog::RESULT_CLICKED_WINDOW = -1;
const int SBGL_Dialog::RESULT_CLICKED_BACKGROUND = -2;

TTF_Font* SBGL_Dialog::msg_font    = NULL;
TTF_Font* SBGL_Dialog::button_font = NULL;

PointF SBGL_Dialog::box_size ( 420, 200 );
PointF SBGL_Dialog::button_size( 150, 40 );
PointF SBGL_Dialog::msg_pos ( 0, 30 );
PointF SBGL_Dialog::button_ok_pos ( -100, -45 );
PointF SBGL_Dialog::button_cancel_pos( 100, -45 );
ColorF SBGL_Dialog::box_color ( 1, 1, 1 );
ColorF SBGL_Dialog::button_color ( 0.5, 0.5, 0.5 );
ColorF SBGL_Dialog::font_color = ColorF( 0, 0, 0 );
ColorF SBGL_Dialog::button_font_color = ColorF( 0, 0 , 0 );

SBGL_Dialog::SBGL_Dialog( const std::string& msg, const std::string& msg_ok, const std::string& msg_cancel )
		throw( std::logic_error )
{
	m_show = false;
	std::fill( vbo_box, vbo_box + 2, 0 );

	if( msg_font == NULL ){
		throw std::logic_error("RF_Dialog::msg_font was not initialized.");
	}
	else if( button_font == NULL ){
		throw std::logic_error("RF_Dialog::button_font was not initialized.");
	}

	data_box.reserve( 4 * 3 * ( 3 + 4 ) );
	
	rect_window = RectF::centered( PointF(), box_size );
	rect_ok 	= RectF::centered( button_ok_pos, button_size );
	rect_cancel = RectF::centered( button_cancel_pos, button_size );

	push_rect( data_box, rect_window, 0, box_color );
	push_rect( data_box, rect_ok, 0.25, button_color );
	push_rect( data_box, rect_cancel, 0.25, button_color );

	index_box = {
		0,  1,  2,
		0,  2,  3,
		4,  5,  6,
		4,  6,  7,
		8,  9, 10,
		8, 10, 11,
	};

	t_msg = std::unique_ptr<SBGL_Text>( new SBGL_Text( msg, msg_font, font_color ) );
	t_msg_ok = std::unique_ptr<SBGL_Text>( new SBGL_Text( msg_ok, button_font, button_font_color ) );
	t_msg_cancel = std::unique_ptr<SBGL_Text>( new SBGL_Text( msg_cancel, button_font, button_font_color ) );

	t_msg->setAlignment( SBGL_Text::SBGL_TEXT_ALIGN_CENTER, SBGL_Text::SBGL_TEXT_ALIGN_CENTER );
}

SBGL_Dialog::~SBGL_Dialog(){

}

void SBGL_Dialog::genBuffers(){
	glGenBuffers( 2, vbo_box );

	glBindBuffer(GL_ARRAY_BUFFER, vbo_box[0] );
	glBufferData(GL_ARRAY_BUFFER, data_box.size()*sizeof(GLfloat), data_box.data(), GL_STATIC_DRAW );
	glBindBuffer(GL_ARRAY_BUFFER, 0 );

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_box[1] );
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_box.size()*sizeof(GLfloat), index_box.data(), GL_STATIC_DRAW );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0 );

	t_msg->genBuffers();
	t_msg_ok->genBuffers();
	t_msg_cancel->genBuffers();
}

void SBGL_Dialog::draw() const {
	if( m_show ){
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
	
		glBindBuffer(GL_ARRAY_BUFFER, vbo_box[0] );
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_box[1] );
		glVertexPointer( 3, GL_FLOAT, ( 3 + 4 ) * sizeof(GLfloat), 0 );
		glColorPointer( 4, GL_FLOAT, ( 3 + 4 ) * sizeof(GLfloat), (void*)( 3 * sizeof(GLfloat) ) );
		glDrawElements( GL_TRIANGLES, index_box.size(), GL_UNSIGNED_INT, 0 );
	
		glDisableClientState(GL_COLOR_ARRAY);
	
		glBindBuffer(GL_ARRAY_BUFFER, 0 );
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0 );
		glDisableClientState(GL_VERTEX_ARRAY);
		
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef( msg_pos.x, msg_pos.y, 0 );
		t_msg->draw();
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslatef( button_ok_pos.x, button_ok_pos.y, 1.0 );
		t_msg_ok->draw();
		glPopMatrix();

		glPushMatrix();
		glTranslatef( button_cancel_pos.x, button_cancel_pos.y, 1.0 );
		t_msg_cancel->draw();
		glPopMatrix();
	}
}

void SBGL_Dialog::show(){
	m_show = true;
}

void SBGL_Dialog::hide(){
	m_show = false;
}

bool SBGL_Dialog::isShown() const {
	return m_show;
}

int SBGL_Dialog::onClick( float x, float y ) const {
	if( rect_ok.hit( x, y ) ){
		return RESULT_ACCEPTED;
	}
	else if( rect_cancel.hit( x, y ) ){
		return RESULT_REJECTED;
	}
	else if( rect_window.hit( x, y ) ){
		return RESULT_CLICKED_WINDOW;
	}
	else {
		return RESULT_CLICKED_BACKGROUND;
	}
}

int SBGL_Dialog::onClick( const PointF& p ) const {
	return onClick( p.x, p.y );
}