#pragma once
/*
	libsoundbag

	Copyright (c) 2017 Togo Nishigaki

	This software released under MIT License
	http://opensource.org/licenses/mit-license.php
*/

#ifndef libsoundbag_zlib_tool_h
#define libsoundbag_zlib_tool_h

#include <new>
#include <stdexcept>
#include <cstdint>
#include <vector>
#include <list>

#include <zlib.h>

typedef uint8_t byte;

namespace soundbag {
  namespace zlib_tool {
    size_t compress( std::vector<byte>& dest, const std::vector<byte>& data, const int level = 6, size_t bufsize = 4096 )
        throw( std::bad_array_new_length, std::invalid_argument, 
               std::domain_error, std::bad_exception, std::runtime_error );

    size_t decompress( std::vector<byte>& dest,  const std::vector<byte>& data, size_t bufsize = 4096 )
      throw( std::bad_array_new_length, std::invalid_argument, 
             std::domain_error, std::bad_exception, std::runtime_error );
  }
}

#endif
