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
    template<class Iterator>
    size_t compress( std::vector<byte>& dest,  const Iterator beg, const Iterator end, const int level = 6, size_t bufsize = 4096 )
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
          throw std::invalid_argument(z.msg);
        }
        else if( init_ret == Z_VERSION_ERROR ){
          throw std::domain_error(z.msg);
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
        size_t i = 0;
        while( i++ < bufsize && iter != end ){ ++iter; }
        if( iter == end ){
          flush = Z_FINISH;
        }
        std::vector<byte> in_buf( before, iter );
        z.next_in = in_buf.data();
        z.avail_in = in_buf.size();
        do{
          z.next_out = out_buf.data();
          z.avail_out = out_buf.size();
          ret = deflate( &z, flush );
          if( ret == Z_STREAM_ERROR ){
            deflateEnd(&z);
            throw std::runtime_error(z.msg);
          }
          if( z.avail_out != 0 ){
            assert( z.avail_out <= out_buf.size() );
            out_buf.resize( out_buf.size() - z.avail_out );
          }
          size_out += out_buf.size();
          buf_pool.insert( buf_pool.end(), std::vector<byte>(bufsize) )->swap(out_buf);
        } while( z.avail_out == 0 );
      }
      deflateEnd(&z);

      std::vector<byte>().swap(dest);
      dest.reserve( size_out );
      for( const auto& b : buf_pool ){
        std::copy( b.begin(), b.end(), std::back_inserter(dest) );
      }

      return dest.size();
    }

    template<class Iterator>
    size_t decompress( std::vector<byte>& dest,  const Iterator beg, const Iterator end, size_t bufsize = 4096 )
            throw( std::bad_array_new_length, std::invalid_argument, std::domain_error, std::bad_exception, std::runtime_error )
    {
      z_stream z;
      z.zalloc = Z_NULL;
      z.zfree  = Z_NULL;
      z.opaque = Z_NULL;
      z.next_in = NULL;
      z.avail_in = 0;
      const int init_ret = inflateInit( &z );
      if( init_ret != Z_OK ){
        if( init_ret == Z_MEM_ERROR ){
          throw std::bad_array_new_length();
        }
        else if( init_ret == Z_STREAM_ERROR ){
          throw std::invalid_argument(z.msg);
        }
        else if( init_ret == Z_VERSION_ERROR ){
          throw std::domain_error(z.msg);
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
        size_t i = 0;
        while( i++ < bufsize && iter != end ){ ++iter; }
        if( iter == end ){
          flush = Z_FINISH;
        }
        std::vector<byte> in_buf( before, iter );
        z.next_in = in_buf.data();
        z.avail_in = in_buf.size();
        do{
          z.next_out = out_buf.data();
          z.avail_out = out_buf.size();
          ret = inflate( &z, flush );
          if( ret == Z_STREAM_ERROR ){
            inflateEnd(&z);
            throw std::runtime_error(z.msg);
          }
          else if( ret == Z_NEED_DICT ){
            inflateEnd(&z);
            throw std::invalid_argument("dictionary is not supported.");
          }
          else if( ret == Z_DATA_ERROR ){
            inflateEnd(&z);
            throw std::invalid_argument("invalid data given");
          }
          else if( ret == Z_MEM_ERROR ){
            throw std::bad_array_new_length();
          }

          if( z.avail_out != 0 ){
            assert( z.avail_out <= out_buf.size() );
            out_buf.resize( out_buf.size() - z.avail_out );
          }
          size_out += out_buf.size();
          buf_pool.insert( buf_pool.end(), std::vector<byte>(bufsize) )->swap(out_buf);
        } while( z.avail_out == 0 );
      }
      inflateEnd(&z);

      std::vector<byte>().swap(dest);
      dest.reserve( size_out );
      for( const auto& b : buf_pool ){
        std::copy( b.begin(), b.end(), std::back_inserter(dest) );
      }

      return dest.size();
    }
  }
}

#endif
