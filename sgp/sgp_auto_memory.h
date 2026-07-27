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
			this->_array = new T[size];
		}
		~AutoArray()
		{
			if(this->_array)
			{
				delete[] this->_array;
			}
		}
	};

	/************************************************************/

	template<typename T, void*(*Alloc)(size_t) = malloc, void(*Dealloc)(void*) = free>
	class AutoCArray : public TAutoArray<T>
	{
	public:
		AutoCArray(size_t size = 1, size_t sizeof_elem=1)
		{
			this->_array = (T*)Alloc( size * sizeof_elem );
		}
		~AutoCArray()
		{
			if(this->_array)
			{
				Dealloc(this->_array);
			}
		}
	};

	/************************************************************/

} // namespace sgp


#include "MemMan.h"

namespace sgp
{
	template<typename T>
	class AutoMMArray : public sgp::TAutoArray<T>
	{
	public:
		AutoMMArray(size_t size, size_t sizeof_elem=1)
		{
			this->_array = (T*)MemAlloc( size * sizeof_elem );
		}
		~AutoMMArray()
		{
			if(this->_array)
			{
				MemFree(this->_array);
			}
		}
	};
}


#endif // _SGP_AUTO_MEMORY_H_
