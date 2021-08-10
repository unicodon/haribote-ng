#pragma once

template<typename T>
struct Vector {
	T vec[4];

	Vector()
	{
		vec[0] = 0;
		vec[1] = 0;
		vec[2] = 0;
		vec[3] = 1;
	}

	Vector(T x, T y, T z, T w)
	{
		vec[0] = x;
		vec[1] = y;
		vec[2] = z;
		vec[3] = w;
	}

	template<typename U>
	Vector(U x, U y, U z)
	{
		vec[0] = static_cast<T>(x);
		vec[1] = static_cast<T>(y);
		vec[2] = static_cast<T>(z);
		vec[3] = 1;
	}

	Vector operator*(const Vector& rhs)
	{
		return {
			vec[0] * rhs.vec[0],
			vec[1] * rhs.vec[1],
			vec[2] * rhs.vec[2],
			vec[3] * rhs.vec[3]
		};
	}
};

template<typename T>
struct Matrix {

};