#pragma once

#ifndef __sb_Bitmap_h__
#define __sb_Bitmap_h__

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