#pragma once
/*
	libsoundbag

	Copyright (c) 2017 Togo Nishigaki

	This software released under MIT License
	http://opensource.org/licenses/mit-license.php
*/

#ifndef libsoundbag_threads_h
#define libsoundbag_threads_h

namespace soundbag {

	/* usage:
		{
			soundbag::try_lock<mutex> locker(_mutex);
			if( locker ){
				...
			}
		}
	*/
	template<class T>
	class try_lock {
		T& m_mutex;
		bool locked;

	public:
		try_lock( T& m ) : m_mutex(m) {
			locked = m_mutex.try_lock();
		}
		virtual ~try_lock() {
			if( locked ){
				m_mutex.unlock();
			}
		}

		operator bool() const { return locked; }
	};
}

#endif