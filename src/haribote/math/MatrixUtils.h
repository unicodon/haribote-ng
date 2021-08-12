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

	Vector operator+(const Vector& rhs)
	{
		return { vec[0] + rhs[0], vec[1] + rhs[1], vec[2] + rhs[2], vec[3] + rhs[3] };
	}

	Vector& operator+=(const Vector& rhs)
	{
		vec[0] += rhs[0];
		vec[1] += rhs[1];
		vec[2] += rhs[2];
		vec[3] += rhs[3];
		return *this;
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

	T& operator[](size_t idx) { return vec[idx]; }
	const T& operator[](size_t idx) const { return vec[idx]; }
};

template<typename T>
struct Matrix {
	T mat[16]; // (vec[0], vec[1], vec[2], vec[3])
	T* data() { return mat; }

	Matrix()
	{
		(*this)[0] = Vector<T>(1, 0, 0, 0);
		(*this)[1] = Vector<T>(0, 1, 0, 0);
		(*this)[2] = Vector<T>(0, 0, 1, 0);
		(*this)[3] = Vector<T>(0, 0, 0, 1);
	}

	//    v0  v1  v2  v3
	// | m00 m10 m20 m30 |
	// | m01 m11 m21 m31 |
	// | m02 m12 m22 m32 |
	// | m03 m13 m23 m33 |
	//
	Matrix(T m00, T m01, T m02, T m03, T m10, T m11, T m12, T m13, T m20, T m21, T m22, T m23, T m30, T m31, T m32, T m33)
	{
		(*this)[0] = Vector<T>(m00, m01, m02, m03);
		(*this)[1] = Vector<T>(m10, m11, m12, m13);
		(*this)[2] = Vector<T>(m20, m21, m22, m23);
		(*this)[3] = Vector<T>(m30, m31, m32, m33);
	}

	Vector<T>& operator[](size_t idx) { return *reinterpret_cast<Vector<T>*>(mat + idx * 4); }
	const Vector<T>& operator[](size_t idx) const { return *reinterpret_cast<const Vector<T>*>(mat + idx * 4); }

	Vector<T> operator*(const Vector<T>& rhs)
	{
		Vector<T> vec;
		for (int i = 0; i < 4; i++) {
			vec[i] = 0;
			for (int j = 0; j < 4; j++) {
				vec[i] += (*this)[j][i] * rhs[j];
			}
		}
		return vec;
	}

	Matrix operator*(const Matrix& rhs)
	{
		Matrix mat;
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

	Matrix& operator*=(const Matrix& rhs)
	{
		*this = *this * rhs;
		return *this;
	}


	void translate(T x, T y, T z)
	{
		(*this)[3] += Vector<T>(x, y, z, 0);
	}

	void rotateX(T rad)
	{
		T c = cos(rad);
		T s = sin(rad);
		Matrix m(
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
		Matrix m(
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
		Matrix m(
			c, s, 0, 0,
			-s, c, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
			);
		*this = *this * m;
	}
};