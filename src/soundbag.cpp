/*
	libsoundbag

	Copyright (c) 2017 Togo Nishigaki

	This software released under MIT License
	http://opensource.org/licenses/mit-license.php
*/
#include <ctime>

#include "soundbag.hpp"

namespace soundbag {
	int32_t xor128() { 
	  static int32_t x = time(0);
	  static int32_t y = time(0);
	  static int32_t z = time(0);
	  static int32_t w = time(0); 
	  uint32_t t;
	 
	  t = x ^ (x << 11);
	  x = y; y = z; z = w;
	  return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
	}
}