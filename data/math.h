#pragma once
#ifndef DATA_MATH_H
#define DATA_MATH_H


namespace Data {
	int log2(int x);
	
	template <class T>
	T min(T const& a, T const& b) { return a < b ? a : b; }

	template <class T>
	T max(T const& a, T const& b) { return a > b ? a : b; }

} // namespace Data


#endif // DATA_MATH_H