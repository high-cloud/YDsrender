#pragma once

#include"YDPrerequisites.h"
#include"Vector.h"

namespace YDSR
{

	class Matrix4
	{
	public:
		Matrix4() :m{} {}

		explicit Matrix4(const Real arr[4][4])
		{
			memcpy(m, arr, 9 * sizeof(Real));
		}

		Matrix4(Real fEntry00, Real fEntry01, Real fEntry02, Real fEntry03,
			Real fEntry10, Real fEntry11, Real fEntry12, Real fEntry13,
			Real fEntry20, Real fEntry21, Real fEntry22, Real fEntry23,
			Real fEntry30, Real fEntry31, Real fEntry32, Real fEntry33)
		{
			m[0][0] = fEntry00;
			m[0][1] = fEntry01;
			m[0][2] = fEntry02;
			m[0][3] = fEntry03;
			m[1][0] = fEntry10;
			m[1][1] = fEntry11;
			m[1][2] = fEntry12;
			m[1][3] = fEntry13;
			m[2][0] = fEntry20;
			m[2][1] = fEntry21;
			m[2][2] = fEntry22;
			m[2][3] = fEntry23;
			m[3][0] = fEntry30;
			m[3][1] = fEntry31;
			m[3][2] = fEntry32;
			m[3][3] = fEntry33;
		}

		Real* operator[](size_t iRow)
		{
			assert(iRow < 4);
			return m[iRow];
		}

		const Real* operator[](size_t iRow) const
		{
			assert(iRow < 4);
			return m[iRow];
		}

		Matrix4 inverse() const;
		Matrix4 transpose() const;

	private:
		Real m[4][4];
	};

	inline Matrix4 Matrix4::transpose() const
	{
		return Matrix4(
			m[0][0], m[1][0], m[2][0], m[3][0],
			m[0][1], m[1][1], m[2][1], m[3][1],
			m[0][2], m[1][2], m[2][2], m[3][2],
			m[0][3], m[1][3], m[2][3], m[3][3]
		);
	}
	Matrix4 operator*(const Matrix4& m, const Matrix4& m2);
	Matrix4 operator+(const Matrix4& m, const Matrix4& m2);
	Matrix4 operator-(const Matrix4& m, const Matrix4& m2);

	Vector4 operator*(const Matrix4& m, const Vector4& v);
}
