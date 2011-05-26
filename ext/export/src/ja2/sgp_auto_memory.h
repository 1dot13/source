#ifndef _SGP_AUTO_MEMORY_H_
#define _SGP_AUTO_MEMORY_H_

namespace sgp
{
	/************************************************************/

	template<typename T>
	class TAutoArray
	{
	public:
		T& operator[](unsigned int index)
		{
			return _array[index];
		}

		T* operator()()
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
		AutoArray(unsigned int size)
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
		AutoCArray(unsigned int size = 1)
		{
			_array = (T*)Alloc( size * sizeof(T) );
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

#endif // _SGP_AUTO_MEMORY_H_
