#ifndef _VFS_ITERATOR_INTERFACE_H_
#define _VFS_ITERATOR_INTERFACE_H_

namespace vfs
{
	template<typename T>
	class VFS_API TIterator
	{
	public:
		class IImplementation
		{
			friend class TIterator<T>;
		public:
			virtual ~IImplementation() {};
			virtual T*					value() = 0;
			virtual void				next() = 0;
		protected:
			virtual IImplementation*	clone() = 0;
		};
	public:
		TIterator() : _obj(NULL), _iter_impl(NULL) {};
		TIterator(IImplementation* impl) : _obj(NULL), _iter_impl(impl)
		{
			if(_iter_impl)
			{
				_obj = _iter_impl->value();
			}
		}
		~TIterator()
		{
			if(_iter_impl) delete _iter_impl;
		};
		TIterator& operator=(TIterator const& t)
		{
			_obj = t._obj;
			_iter_impl = NULL;
			if(t._iter_impl)
			{
				_iter_impl = t._iter_impl->clone();
			}
			return *this;
		}
		//////////////////////////////
		T*		value()		{ return _obj; };
		bool	end()		{ return _obj == NULL; };
		void	next()
		{
			if(_iter_impl)
			{
				_iter_impl->next();
				_obj = _iter_impl->value();
				if(!_obj)
				{
					delete _iter_impl;
					_iter_impl = NULL;
				}
			}
		}
	private:
		T*					_obj;
		IImplementation*	_iter_impl;
	};
}

#endif // _VFS_ITERATOR_INTERFACE_H_
