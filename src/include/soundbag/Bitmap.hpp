#pragma once
/*
	libsoundbag

	Copyright (c) 2017 Togo Nishigaki

	This software released under MIT License
	http://opensource.org/licenses/mit-license.php
*/

#ifndef libsoundbag_Bitmap_h
#define libsoundbag_Bitmap_h

#include <cstdint>
#include <stdexcept>

#include "soundbag.hpp"

namespace soundbag {
	struct Bitmap{
		enum class BitmapFormat {
			RGB,
			RGBA,
		};
		BitmapFormat format;
		int32_t width;
		int32_t height;
		std::vector<byte> data;

		inline Bitmap(){
			format = BitmapFormat::RGB;
			width = height = 0;
		}

	#ifdef _SDL_H
		inline Bitmap( SDL_Surface* const s_img ) throw( std::invalid_argument ){
			if( s_img == NULL ){
				throw std::invalid_argument("NULL surface given.");
			}

			width = s_img->w;
			height = s_img->h;

			if( s_img->format->format == SDL_PIXELFORMAT_RGB888 ||
				s_img->format->format == SDL_PIXELFORMAT_RGB24 ){
				format = BitmapFormat::RGB;
				std::vector<byte>( (byte*)s_img->pixels, (byte*)s_img->pixels + ( width * height * 3 ) ).swap( data );

			}
			else if( s_img->format->format == SDL_PIXELFORMAT_RGBA8888 ){
				format = BitmapFormat::RGBA;
				std::vector<byte>( (byte*)s_img->pixels, (byte*)s_img->pixels + ( width * height * 3 ) ).swap( data );
			}
			else {
				format = BitmapFormat::RGBA;

				SDL_PixelFormat s_fmt;
				s_fmt.format = SDL_PIXELFORMAT_RGBA8888;
				s_fmt.palette = NULL;
				s_fmt.BitsPerPixel = 32;
				s_fmt.BytesPerPixel = 4;
				s_fmt.Rmask = 0xFF000000;
				s_fmt.Gmask = 0x00FF0000;
				s_fmt.Bmask = 0x0000FF00;
				s_fmt.Amask = 0x000000FF;
				SDL_Surface* c_surface = SDL_ConvertSurface( s_img, &s_fmt, 0 );
				std::vector<byte>( (byte*)c_surface->pixels, (byte*)c_surface->pixels + ( width * height * 4 ) ).swap( data );
				SDL_FreeSurface( c_surface );
			}
		}
	#endif
	};
}

#endif