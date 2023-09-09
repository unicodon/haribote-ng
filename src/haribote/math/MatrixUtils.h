#pragma once

#include <math.h>

template<typename T>
struct VectorBase {
	T vec[4];

	VectorBase()
	{
		vec[0] = 0;
		vec[1] = 0;
		vec[2] = 0;
		vec[3] = 1;
	}

	VectorBase(T x, T y, T z, T w = 1)
	{
		vec[0] = x;
		vec[1] = y;
		vec[2] = z;
		vec[3] = w;
	}

	VectorBase operator+(const VectorBase& rhs)
	{
		return { vec[0] + rhs[0], vec[1] + rhs[1], vec[2] + rhs[2], vec[3] + rhs[3] };
	}

	VectorBase& operator+=(const VectorBase& rhs)
	{
		vec[0] += rhs[0];
		vec[1] += rhs[1];
		vec[2] += rhs[2];
		vec[3] += rhs[3];
		return *this;
	}

	VectorBase& operator-=(const VectorBase& rhs)
	{
		vec[0] -= rhs[0];
		vec[1] -= rhs[1];
		vec[2] -= rhs[2];
		vec[3] -= rhs[3];
		return *this;
	}

	T operator*(const VectorBase& rhs)
	{
		return
			vec[0] * rhs.vec[0] +
			vec[1] * rhs.vec[1] +
			vec[2] * rhs.vec[2] +
			vec[3] * rhs.vec[3];
	}

	VectorBase& operator*=(T scalar)
	{
		vec[0] *= scalar;
		vec[1] *= scalar;
		vec[2] *= scalar;
		vec[3] *= scalar;
		return *this;
	}

	T& operator[](size_t idx) { return vec[idx]; }
	const T& operator[](size_t idx) const { return vec[idx]; }
};

template<typename T>
struct MatrixBase {
	T mat[16]; // (vec[0], vec[1], vec[2], vec[3])
	T* data() { return mat; }
	const T* data() const { return mat; }

	MatrixBase()
	{
		(*this)[0] = VectorBase<T>(1, 0, 0, 0);
		(*this)[1] = VectorBase<T>(0, 1, 0, 0);
		(*this)[2] = VectorBase<T>(0, 0, 1, 0);
		(*this)[3] = VectorBase<T>(0, 0, 0, 1);
	}

	//    v0  v1  v2  v3
	// | m00 m10 m20 m30 |
	// | m01 m11 m21 m31 |
	// | m02 m12 m22 m32 |
	// | m03 m13 m23 m33 |
	//
	MatrixBase(T m00, T m01, T m02, T m03, T m10, T m11, T m12, T m13, T m20, T m21, T m22, T m23, T m30, T m31, T m32, T m33)
	{
		(*this)[0] = VectorBase<T>(m00, m01, m02, m03);
		(*this)[1] = VectorBase<T>(m10, m11, m12, m13);
		(*this)[2] = VectorBase<T>(m20, m21, m22, m23);
		(*this)[3] = VectorBase<T>(m30, m31, m32, m33);
	}

	VectorBase<T>& operator[](size_t idx) { return *reinterpret_cast<VectorBase<T>*>(mat + idx * 4); }
	const VectorBase<T>& operator[](size_t idx) const { return *reinterpret_cast<const VectorBase<T>*>(mat + idx * 4); }

	VectorBase<T> operator*(const VectorBase<T>& rhs)
	{
		VectorBase<T> vec;
		for (int i = 0; i < 4; i++) {
			vec[i] = 0;
			for (int j = 0; j < 4; j++) {
				vec[i] += (*this)[j][i] * rhs[j];
			}
		}
		return vec;
	}

	MatrixBase operator*(const MatrixBase& rhs)
	{
		MatrixBase mat;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				mat[j][i] = 0;
				for (int k = 0; k < 4; k++) {
					mat[j][i] += (*this)[k][i] * rhs[j][k];
				}
			}
		}
		return mat;
	}

	MatrixBase& operator*=(const MatrixBase& rhs)
	{
		*this = *this * rhs;
		return *this;
	}

	void translate(T x, T y, T z)
	{
		MatrixBase mat;
		mat[3] = VectorBase<T>(x, y, z);
		*this = *this * mat;
	}

	void translate(const VectorBase<T>& vec)
	{
		MatrixBase mat;
		mat[3] = vec;
		*this = *this * mat;
	}

	void rotateX(T rad)
	{
		T c = cos(rad);
		T s = sin(rad);
		MatrixBase m(
			1, 0, 0, 0,
			0, c, -s, 0,
			0, s, c, 0,
			0, 0, 0, 1
			);
		*this = *this * m;
	}

	void rotateY(T rad)
	{
		T c = cos(rad);
		T s = sin(rad);
		MatrixBase m(
			c, 0, -s, 0,
			0, 1, 0, 0,
			s, 0, c, 0,
			0, 0, 0, 1
			);
		*this = *this * m;
	}

	void rotateZ(T rad)
	{
		T c = cos(rad);
		T s = sin(rad);
		MatrixBase m(
			c, s, 0, 0,
			-s, c, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
			);
		*this = *this * m;
	}

	void scale(T scale)
	{
		scaleX(scale);
		scaleY(scale);
		scaleZ(scale);
	}

	void scaleX(T scale)
	{
		(*this)[0] *= scale;
	}

	void scaleY(T scale)
	{
		(*this)[1] *= scale;
	}

	void scaleZ(T scale)
	{
		(*this)[2] *= scale;
	}
};

using Vector = VectorBase<float>;
using Matrix = MatrixBase<float>;
