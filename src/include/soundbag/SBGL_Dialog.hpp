#pragma once
#pragma execution_character_set("utf-8")

#ifndef rise_and_fall_RF_Dialog_h
#define rise_and_fall_RF_Dialog_h

#include <vector>
#include <memory>
#include <string>
#include <exception>
#include <GL/gl.h>
#include "soundbag/Point.hpp"
#include "soundbag/Color.hpp"
#include "soundbag/Rect.hpp"

namespace soundbag {
	class SBGL_Text;

	class SBGL_Dialog {
		GLuint vbo_box[2];

		std::vector<GLfloat> data_box;
		std::vector<GLuint> index_box;

		bool m_show;

		RectF rect_window;
		RectF rect_ok;
		RectF rect_cancel;

		std::unique_ptr<SBGL_Text> t_msg;
		std::unique_ptr<SBGL_Text> t_msg_ok;
		std::unique_ptr<SBGL_Text> t_msg_cancel;

	public:
		static const int RESULT_ACCEPTED;
		static const int RESULT_REJECTED;
		static const int RESULT_CLICKED_WINDOW;
		static const int RESULT_CLICKED_BACKGROUND;

		static TTF_Font* msg_font;
		static TTF_Font* button_font;

		static PointF box_size;
		static PointF button_size;
		static PointF msg_pos;
		static PointF button_ok_pos;
		static PointF button_cancel_pos;
		static ColorF box_color;
		static ColorF button_color;
		static ColorF font_color;
		static ColorF button_font_color;

		SBGL_Dialog( const std::string& msg, 
				   const std::string& msg_ok = "OK", const std::string& msg_cancel = "Cancel" )
			 	throw( std::logic_error )
			;
		~SBGL_Dialog();

		void genBuffers();
		void draw() const;

		void show();
		void hide();
		bool isShown() const;

		int onClick( float x, float y ) const;
		int onClick( const PointF& relative_position ) const;

	};
}

#endif