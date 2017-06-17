#include "gtest/gtest.h"

#include <iostream>
#include <iomanip>

#include "soundbag/soundbag.hpp"
#include "soundbag/zlib_tool.hpp"

using namespace soundbag;

TEST( zlib_tool, compress ){
  std::vector<byte> data;
  std::vector<byte> dest;
  constexpr size_t data_size = 1024 * 1024;
  data.reserve( data_size );
  for( size_t i = 0; i < data_size; ++i ){
    data.push_back( (byte)( (long double)xor128() / UINT32_MAX * 16 ) * 16 );
  }
  
  try{
    zlib_tool::compress( dest, data.begin(), data.end(), 9 );
  }
  catch( std::exception& e ){
    FAIL() << "Error while compression: " << e.what();
  }

  std::cout << std::dec << data.size() << " -> " << dest.size() << std::endl;

  std::vector<byte> data2;
  try{
    zlib_tool::decompress( data2, dest.begin(), dest.end() );
  }
  catch( std::exception& e ){
    FAIL() << "Error while decompression: " << e.what();
  }

  ASSERT_TRUE( data.size() == data2.size() && std::equal(data.begin(),data.end(),data2.begin()) ) << "inflated data is not match.";
}
