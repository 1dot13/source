/* 
 * bfVFS : vfs/Core/Interface/vfs_iterator_interface.h
 *  - generic interface to iterate over files in locations
 *
 * Copyright (C) 2008 - 2010 (BF) john.bf.smith@googlemail.com
 * 
 * This file is part of the bfVFS library
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

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
