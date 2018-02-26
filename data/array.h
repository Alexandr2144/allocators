#pragma once
#ifndef DATA_ARRAY_H
#define DATA_ARRAY_H


namespace Data {
	template <class T>
	struct Array {
		T* begin;
		T* end;
	};
	template <class T>
	using Array_CRef = Array<T> const&;

	template <class T>
	size_t isEmpty(Array_CRef<T> a);

	template <class T>
	size_t count(Array_CRef<T> a);

	template <class T>
	size_t size(Array_CRef<T> a);


	template <class T>
	class ArrayCollection {
	public:
		ArrayCollection(Array_CRef<T> a);

		T* begin();
		T* end();

	private:
		Array_CRef<T> m_array;
	};

	template <class T>
	ArrayCollection<T> iterate(Array_CRef<T> a);

} // namespace Data


#include "data/hpp/array.hpp"

#endif // DATA_ARRAY_H