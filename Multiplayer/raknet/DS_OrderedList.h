/// \file
/// \brief \b [Internal] Quicksort ordered list.
///
/// This file is part of RakNet Copyright 2003 Kevin Jenkins.
///
/// Usage of RakNet is subject to the appropriate license agreement.
/// Creative Commons Licensees are subject to the
/// license found at
/// http://creativecommons.org/licenses/by-nc/2.5/
/// Single application licensees are subject to the license found at
/// http://www.jenkinssoftware.com/SingleApplicationLicense.html
/// Custom license users are subject to the terms therein.
/// GPL license users are subject to the GNU General Public
/// License as published by the Free
/// Software Foundation; either version 2 of the License, or (at your
/// option) any later version.

#include "DS_List.h"
#include "RakMemoryOverride.h"
#include "Export.h"

#ifndef __ORDERED_LIST_H
#define __ORDERED_LIST_H

/// The namespace DataStructures was only added to avoid compiler errors for commonly named data structures
/// As these data structures are stand-alone, you can use them outside of RakNet for your own projects if you wish.
namespace DataStructures
{
	template <class key_type, class data_type>
	int defaultOrderedListComparison(const key_type &a, const data_type &b)
	{
		if (a<b) return -1; if (a==b) return 0; return 1;
	}

	/// \note IMPORTANT! If you use defaultOrderedListComparison then call IMPLEMENT_DEFAULT_COMPARISON or you will get an unresolved external linker error.
	template <class key_type, class data_type, int (*default_comparison_function)(const key_type&, const data_type&)=defaultOrderedListComparison<key_type, data_type> >
	class RAK_DLL_EXPORT OrderedList
	{
	public:
		static void IMPLEMENT_DEFAULT_COMPARISON(void) {DataStructures::defaultOrderedListComparison<key_type, data_type>(key_type(),data_type());}

		OrderedList();
		~OrderedList();
		OrderedList( const OrderedList& original_copy );
		OrderedList& operator= ( const OrderedList& original_copy );

		/// comparisonFunction must take a key_type and a data_type and return <0, ==0, or >0
		/// If the data type has comparison operators already defined then you can just use defaultComparison
		bool HasData(const key_type &key, int (*cf)(const key_type&, const data_type&)=default_comparison_function) const;
		// GetIndexFromKey returns where the insert should go at the same time checks if it is there
		unsigned GetIndexFromKey(const key_type &key, bool *objectExists, int (*cf)(const key_type&, const data_type&)=default_comparison_function) const;
		data_type GetElementFromKey(const key_type &key, int (*cf)(const key_type&, const data_type&)=default_comparison_function) const;
		bool GetElementFromKey(const key_type &key, data_type &element, int (*cf)(const key_type&, const data_type&)=default_comparison_function) const;
		unsigned Insert(const key_type &key, const data_type &data, bool assertOnDuplicate, int (*cf)(const key_type&, const data_type&)=default_comparison_function);
		unsigned Remove(const key_type &key, int (*cf)(const key_type&, const data_type&)=default_comparison_function);
		unsigned RemoveIfExists(const key_type &key, int (*cf)(const key_type&, const data_type&)=default_comparison_function);
		data_type& operator[] ( const unsigned int position ) const;
		void RemoveAtIndex(const unsigned index);
		void InsertAtIndex(const data_type &data, const unsigned index);
		void InsertAtEnd(const data_type &data);
		void RemoveFromEnd(const unsigned num=1);
		void Clear(bool doNotDeallocate=false);
		unsigned Size(void) const;
	
	protected:
		DataStructures::List<data_type> orderedList;
	};

	template <class key_type, class data_type, int (*default_comparison_function)(const key_type&, const data_type&)>
	OrderedList<key_type, data_type, default_comparison_function>::OrderedList()
	{
	}

	template <class key_type, class data_type, int (*default_comparison_function)(const key_type&, const data_type&)>
	OrderedList<key_type, data_type, default_comparison_function>::~OrderedList()
	{
		Clear();
	}

	template <class key_type, class data_type, int (*default_comparison_function)(const key_type&, const data_type&)>
	OrderedList<key_type, data_type, default_comparison_function>::OrderedList( const OrderedList& original_copy )
	{
		orderedList=original_copy.orderedList;
	}

	template <class key_type, class data_type, int (*default_comparison_function)(const key_type&, const data_type&)>
	OrderedList<key_type, data_type, default_comparison_function>& OrderedList<key_type, data_type, default_comparison_function>::operator= ( const OrderedList& original_copy )
	{
		orderedList=original_copy.orderedList;
		return *this;
	}

	template <class key_type, class data_type, int (*default_comparison_function)(const key_type&, const data_type&)>
	bool OrderedList<key_type, data_type, default_comparison_function>::HasData(const key_type &key, int (*cf)(const key_type&, const data_type&)) const
	{
		bool objectExists;
		unsigned index;
		index = GetIndexFromKey(key, &objectExists, cf);
		return objectExists;
	}

	template <class key_type, class data_type, int (*default_comparison_function)(const key_type&, const data_type&)>
	data_type OrderedList<key_type, data_type, default_comparison_function>::GetElementFromKey(const key_type &key, int (*cf)(const key_type&, const data_type&)) const
	{
		bool objectExists;
		unsigned index;
		index = GetIndexFromKey(key, &objectExists, cf);
		RakAssert(objectExists);
		return orderedList[index];
	}
	template <class key_type, class data_type, int (*default_comparison_function)(const key_type&, const data_type&)>
	bool OrderedList<key_type, data_type, default_comparison_function>::GetElementFromKey(const key_type &key, data_type &element, int (*cf)(const key_type&, const data_type&)) const
	{
		bool objectExists;
		unsigned index;
		index = GetIndexFromKey(key, &objectExists, cf);
		if (objectExists)
			element = orderedList[index];
		return objectExists;
	}
	template <class key_type, class data_type, int (*default_comparison_function)(const key_type&, const data_type&)>
	unsigned OrderedList<key_type, data_type, default_comparison_function>::GetIndexFromKey(const key_type &key, bool *objectExists, int (*cf)(const key_type&, const data_type&)) const
	{
		int index, upperBound, lowerBound;
		int res;

		if (orderedList.Size()==0)
		{
			*objectExists=false;
			return 0;
		}

		upperBound=(int)orderedList.Size()-1;
		lowerBound=0;
		index = (int)orderedList.Size()/2;

#ifdef _MSC_VER
	#pragma warning( disable : 4127 ) // warning C4127: conditional expression is constant
#endif
		while (1)
		{
			res = cf(key,orderedList[index]);
			if (res==0)
			{
				*objectExists=true;
				return index;
			}
			else if (res<0)
			{
				upperBound=index-1;
			}
			else// if (res>0)
			{
				lowerBound=index+1;
			}

			index=lowerBound+(upperBound-lowerBound)/2;

			if (lowerBound>upperBound)
			{
				*objectExists=false;
				return lowerBound; // No match
			}
		}
	}

	template <class key_type, class data_type, int (*default_comparison_function)(const key_type&, const data_type&)>
	unsigned OrderedList<key_type, data_type, default_comparison_function>::Insert(const key_type &key, const data_type &data, bool assertOnDuplicate, int (*cf)(const key_type&, const data_type&))
	{
		(void) assertOnDuplicate;
		bool objectExists;
		unsigned index;
		index = GetIndexFromKey(key, &objectExists, cf);

		// Don't allow duplicate insertion.
		if (objectExists)
		{
			// This is usually a bug!  Use InsertAllowDuplicate if you want duplicates
			RakAssert(assertOnDuplicate==false);
			return (unsigned)-1;
		}

		if (index>=orderedList.Size())
		{
			orderedList.Insert(data);
			return orderedList.Size()-1;
		}
		else
		{
			orderedList.Insert(data,index);
			return index;
		}		
	}

	template <class key_type, class data_type, int (*default_comparison_function)(const key_type&, const data_type&)>
	unsigned OrderedList<key_type, data_type, default_comparison_function>::Remove(const key_type &key, int (*cf)(const key_type&, const data_type&))
	{
		bool objectExists;
		unsigned index;
		index = GetIndexFromKey(key, &objectExists, cf);

		// Can't find the element to remove if this assert hits
	//	RakAssert(objectExists==true);
		if (objectExists==false)
		{
			RakAssert(objectExists==true);
			return 0;
		}

		orderedList.RemoveAtIndex(index);
		return index;
	}

	template <class key_type, class data_type, int (*default_comparison_function)(const key_type&, const data_type&)>
	unsigned OrderedList<key_type, data_type, default_comparison_function>::RemoveIfExists(const key_type &key, int (*cf)(const key_type&, const data_type&))
	{
		bool objectExists;
		unsigned index;
		index = GetIndexFromKey(key, &objectExists, cf);

		// Can't find the element to remove if this assert hits
		if (objectExists==false)
			return 0;

		orderedList.RemoveAtIndex(index);
		return index;
	}

	template <class key_type, class data_type, int (*default_comparison_function)(const key_type&, const data_type&)>
	void OrderedList<key_type, data_type, default_comparison_function>::RemoveAtIndex(const unsigned index)
	{
		orderedList.RemoveAtIndex(index);
	}

	template <class key_type, class data_type, int (*default_comparison_function)(const key_type&, const data_type&)>
		void OrderedList<key_type, data_type, default_comparison_function>::InsertAtIndex(const data_type &data, const unsigned index)
	{
		orderedList.Insert(data, index);
	}

	template <class key_type, class data_type, int (*default_comparison_function)(const key_type&, const data_type&)>
		void OrderedList<key_type, data_type, default_comparison_function>::InsertAtEnd(const data_type &data)
	{
		orderedList.Insert(data);
	}

	template <class key_type, class data_type, int (*default_comparison_function)(const key_type&, const data_type&)>
		void OrderedList<key_type, data_type, default_comparison_function>::RemoveFromEnd(const unsigned num)
	{
		orderedList.RemoveFromEnd(num);
	}

	template <class key_type, class data_type, int (*default_comparison_function)(const key_type&, const data_type&)>
	void OrderedList<key_type, data_type, default_comparison_function>::Clear(bool doNotDeallocate)
	{
		orderedList.Clear(doNotDeallocate);
	}

	template <class key_type, class data_type, int (*default_comparison_function)(const key_type&, const data_type&)>
	data_type& OrderedList<key_type, data_type, default_comparison_function>::operator[]( const unsigned int position ) const
	{
		return orderedList[position];
	}

	template <class key_type, class data_type, int (*default_comparison_function)(const key_type&, const data_type&)>
	unsigned OrderedList<key_type, data_type, default_comparison_function>::Size(void) const
	{
		return orderedList.Size();
	}
}

#endif
