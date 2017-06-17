#include <new>
#include <stdexcept>
#include <cstdint>
#include <list>
#include <cassert>

#include "soundbag/zlib_tool.hpp"
#include <zlib.h>

using namespace soundbag;

template<class Iterator>
size_t zlib_tool<Iterator>::compress( std::vector<byte>& dest,  const Iterator beg, const Iterator end, const int level, size_t bufsize )
        throw( std::bad_array_new_length, std::invalid_argument, std::domain_error, std::bad_exception, std::runtime_error )
{
  z_stream z;
  z.zalloc = Z_NULL;
  z.zfree  = Z_NULL;
  z.opaque = Z_NULL;
  const int init_ret = deflateInit( &z, level );
  if( init_ret != Z_OK ){
    if( init_ret == Z_MEM_ERROR ){
      throw std::bad_array_new_length();
    }
    else if( init_ret == Z_STREAM_ERROR ){
      throw std::invalid_argument();
    }
    else if( init_ret == Z_VERSION_ERROR ){
      throw std::domain_error();
    }
    else {
      throw std::bad_exception();
    }
  }

  Iterator iter = beg;
  std::vector<byte> out_buf(bufsize);
  std::list< std::vector<byte> > buf_pool;
  size_t size_out = 0;
  int flush = Z_NO_FLUSH;
  int ret = Z_OK;
  while( ret != Z_STREAM_END ){
    const Iterator before = iter;
    for( size_t i = 0; i < bufsize; ++i ){
      ++iter;
      if( iter == end ){
        flush = Z_FINISH;
        break;
      }
    }
    std::vector<byte> in_buf( before, iter );
    z.next_in = in_buf.data();
    z.avail_in = in_buf.size();
    do{
      z.next_out = out_buf.data();
      z.avail_out = out_buf.size();
      ret = deflate( &z, flush );
      if( ret == Z_BUF_ERROR ){
        throw std::runtime_error();
      }
      if( z.avail_out != 0 ){
        assert( z.avail_out <= out_buf.size() );
        out_buf.resize( out_buf.size() - z.avail_out );
      }
      size_out += out_buf.size();
      const int next_reserve = ( z.avail_in != 0 ) ? bufsize : 0;
      buf_pool.insert( buf_pool.end(), std::vector<byte>(next_reserve) )->swap(out_buf);
    } while( z.avail_in != 0 );
  }
  deflateEnd(&z);

  std::vector<byte>().swap(dest);
  dest.reserve( size_out );
  for( const auto& b : buf_pool ){
    std::copy( b.begin(), b.end(), std::back_inserter(dest) );
  }

  return dest.size();
}
