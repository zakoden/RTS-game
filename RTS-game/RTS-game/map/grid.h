#pragma once

#include <functional>
#include <vector>

#include "point.h"

using std::vector;

template<typename T>
class Grid : public vector<vector<T>> {
public:
	inline Grid() = default;

	inline Grid(size_t height, size_t width);
	inline Grid(size_t height, size_t width, T default_value);

	inline vector<T>& operator[](size_t i);
	inline const vector<T>& operator[](size_t i) const;
	inline T& operator[](Point point);
	inline const T& operator[](Point point) const;
};

template<typename T>
inline Grid<T>::Grid(size_t height, size_t width) {
	vector<vector<T>>::resize(height, vector<T>(width));
}

template<typename T>
inline Grid<T>::Grid(size_t height, size_t width, T default_value) {
	vector<vector<T>>::resize(height, vector<T>(width, default_value));
}

template<typename T>
inline vector<T>& Grid<T>::operator[](size_t i) {
	return vector<vector<T>>::operator[](i);
}

template<typename T>
inline const vector<T>& Grid<T>::operator[](size_t i) const {
	return vector<vector<T>>::operator[](i);
}

template<typename T>
inline T& Grid<T>::operator[](Point point) {
	return operator[](point.y)[point.x];
}

template<typename T>
inline const T& Grid<T>::operator[](Point point) const {
	return operator[](point.y)[point.x];
}
