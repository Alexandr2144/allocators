#include "data/array.h"


namespace Data {
	// ------------------------------------------------------------------------
	template <class T>
	size_t count(Array_CRef<T> a)
	{
		return a.end - a.begin;
	}

	// ------------------------------------------------------------------------
	template <class T>
	size_t size(Array_CRef<T> a)
	{
		return sizeof(T) * count(a);
	}

	// ------------------------------------------------------------------------
	template <class T>
	size_t isEmpty(Array_CRef<T> a)
	{
		return (count(a) == 0);
	}

	// ------------------------------------------------------------------------
	template <class T>
	ArrayCollection<T>::ArrayCollection(Array_CRef<T> a)
		: m_array(a)
	{
	}

	// ------------------------------------------------------------------------
	template <class T>
	T* ArrayCollection<T>::begin()
	{
		return m_array.begin;
	}

	// ------------------------------------------------------------------------
	template <class T>
	T* ArrayCollection<T>::end()
	{
		return m_array.end;
	}

	// ------------------------------------------------------------------------
	template <class T>
	ArrayCollection<T> iterate(Array_CRef<T> a);

} // namespace Data