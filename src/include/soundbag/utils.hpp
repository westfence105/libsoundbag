
#pragma once

#ifndef __tools__base_h_
#define __tools__base_h_

#include <iostream>
#include <cmath>
#include <vector>
#include <array>

#define numof(array)  (sizeof (array) / sizeof *(array))

namespace soundbag {
	
	static inline constexpr void* BUFFER_OFFSET( intptr_t t ){
		return (char*)NULL + t;
	}
	
	template<class T, class... Types>
	static inline constexpr std::array<T,sizeof...(Types)> make_array( const Types&... args ){
		return {{ T(args)... }};
	}
	
	static inline constexpr double simple_pow( double a, int b ){
		return b == 0 ? 1 : ( b > 0 ? a*simple_pow(a,b-1) : a/simple_pow(a,b+1) );
	}
	
	static inline constexpr double p2( double a ){ return a * a; }
	static inline constexpr double p3( double a ){ return a * a * a; }
	
	template<class T>
	static inline void swap( T &a, T &b ){
		T tmp = a;
		a = b;
		b = tmp;
	}

	template<class T>
	static inline bool in_range(T n, T min, T max){ //範囲判定
			if( min > max ){
				swap( min, max );
			}
			if( min <= n && n <= max ){ return true; }
			else return false;
	}
	
	static inline bool approx(double a,double b,double r=0.002){
			if( fabs(a-b) <= r ) return true;
			else return false;
	}
	
	template <class T>
	static inline T bigger( T a, T b ){
		if( a > b ) return a;
		else return b;
	}
	
	template <class T>
	static inline T smaller( T a, T b ){
		if( a < b ) return a;
		else return b;
	}

	class is_numeric {
	public:
		static constexpr int NUMERIC_TYPE_UNSIGNED_INT = 0;
		static constexpr int NUMERIC_TYPE_SIGNED_INT = 1;
		static constexpr int NUMERIC_TYPE_DECIMAL = 3;
	
	private:
		int base;
		int type;
	
	public:
		inline is_numeric( int _base = 10, int _type = NUMERIC_TYPE_DECIMAL ) : base(_base ), type(_type ) {}
		inline bool operator()( char c ){
			if( '0' <= c && c < ( '0' + ( ( base < 10 ) ? base : 10 ) ) ){
				return true;
			}
			else if( ( type & 1 ) && ( c == '-' || c == '+' ) ){
				return true;
			}
			else if( ( type & 2 ) && ( c == '.' || c == 'e' || c == 'E' ) ){
				return true;
			}
			else if( base == 2 && c == 'b' ){
				return true;
			}
			else if( 10 < base ){
				if ( 'a' <= c && c < 'a' + ( base - 10 ) ){
					return true;
				}
				else if ( 'A' <= c && c < 'A' + ( base - 10 ) ){
					return true;
				}
				else if( base == 16 && c == 'x' ){
					return true;
				}
			}
			return false;
		}
		inline bool operator()( const std::string& str ){
			for( char c : str ){
				if( ! (*this)( c ) ){ return false; }
			}
			return true;
		}
	};

	
	template<typename T, T First>
	static inline constexpr bool match_any_of( const T& c ) {
		return ( c == First );
	}
	
	template<typename T, T First, char Second, T ...rest>
	static inline constexpr bool match_any_of( const T& c ) {
		return match_any_of<T,First>( c ) || match_any_of<T,Second,rest...>( c );
	}
	

	class ordinal {
		int num;

	public:
		inline constexpr ordinal( int _num ) : num(_num ) {}
		inline virtual ~ordinal() {}

		friend inline std::ostream& operator<< ( std::ostream& os, const ordinal& obj ){
			os << obj.num;
			if( obj.num == 11 || obj.num == 12 ){
				os << "th";
			}
			else {
				const int last_digit = obj.num % 10;
				switch( last_digit ){
					case 1:
						os << "st";
						break;
		
					case 2:
						os << "nd";
						break;
		
					case 3:
						os << "rd";
						break;
		
					default:
						os << "th";
						break;
				}
			}
			return os;
		}
	};
}

#endif
