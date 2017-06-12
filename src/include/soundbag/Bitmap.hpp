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

#include "soundbag.hpp"

namespace soundbag {
	struct Bitmap{
		byte compressed;
		int32_t width;
		int32_t height;
		std::vector<byte> data;
	};
}

#endif