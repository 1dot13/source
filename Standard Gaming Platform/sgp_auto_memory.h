#ifndef _SGP_AUTO_MEMORY_H_
#define _SGP_AUTO_MEMORY_H_

namespace sgp
{
	/************************************************************/

	template<typename T>
	class TAutoArray
	{
	public:
		T& operator[](size_t index)
		{
			return _array[index];
		}

		T* operator()()
		{
			return _array;
		}
		T* operator->()
		{
			return _array;
		}
		T& operator*()
		{
			return *_array;
		}
		bool operator!()
		{
			// NULL == _array
			return !_array;
		}

		T* release()
		{
			T* tmp = _array;
			_array = NULL;
			return tmp;
		}
	protected:
		T* _array;
	};

	/************************************************************/

	template<typename T>
	class AutoArray : public TAutoArray<T>
	{
	public:
		AutoArray(size_t size)
		{
			_array = new T[size];
		}
		~AutoArray()
		{
			if(_array)
			{
				delete[] _array;
			}
		}
	};

	/************************************************************/

	template<typename T, typename void*(*Alloc)(size_t) = malloc, typename void(*Dealloc)(void*) = free>
	class AutoCArray : public TAutoArray<T>
	{
	public:
		AutoCArray(size_t size = 1, size_t sizeof_elem=1)
		{
			_array = (T*)Alloc( size * sizeof_elem );
		}
		~AutoCArray()
		{
			if(_array)
			{
				Dealloc(_array);
			}
		}
	};

	/************************************************************/

} // namespace sgp

#ifdef JA2

#include "MemMan.h"

namespace sgp
{
	template<typename T>
	class AutoMMArray : public sgp::TAutoArray<T>
	{
	public:
		AutoMMArray(size_t size, size_t sizeof_elem=1)
		{
			_array = (T*)MemAlloc( size * sizeof_elem );
		}
		~AutoMMArray()
		{
			if(_array)
			{
				MemFree(_array);
			}
		}
	};
}

#endif

#endif // _SGP_AUTO_MEMORY_H_
