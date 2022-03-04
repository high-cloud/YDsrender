#pragma once
#include<iostream>
#include<cassert>
#include"YDPrerequisites.h"
#include"Math.h"

namespace YDSR
{

	template <int dims, typename T> struct VectorBase
	{
		VectorBase() {}
		VectorBase(T _x, T _y)
		{
			static_assert(dims > 1, "must have at least 2 dimensions");
			data[0] = _x; data[1] = _y;
		}
		VectorBase(T _x, T _y, T _z)
		{
			static_assert(dims > 2, "must have at least 3 dimensions");
			data[0] = _x; data[1] = _y; data[2] = _z;
		}
		VectorBase(T _x, T _y, T _z, T _w)
		{
			static_assert(dims > 3, "must have at least 4 dimensions");
			data[0] = _x; data[1] = _y; data[2] = _z; data[3] = _w;
		}
		T data[dims];
		T* ptr() { return data; }
		const T* ptr() const { return data; }
	};

	template <> struct  VectorBase<2, Real>
	{
		VectorBase() {}
		VectorBase(Real _x, Real _y) : x(_x), y(_y) {}
		Real x, y;
		Real* ptr() { return &x; }
		const Real* ptr() const { return &x; }


		/** Calculates the 2 dimensional cross-product of 2 vectors, which results
			in a single floating point value which is 2 times the area of the triangle.
		*/
		Real crossProduct(const VectorBase& rkVector) const
		{
			return x * rkVector.y - y * rkVector.x;
		}


		// special points
		static const Vector2 ZERO;
		static const Vector2 UNIT_X;
		static const Vector2 UNIT_Y;
		static const Vector2 NEGATIVE_UNIT_X;
		static const Vector2 NEGATIVE_UNIT_Y;
		static const Vector2 UNIT_SCALE;
	};

	template <> struct VectorBase<3, Real>
	{
		VectorBase() {}
		VectorBase(Real _x, Real _y, Real _z) : x(_x), y(_y), z(_z) {}
		Real x, y, z;
		Real* ptr() { return &x; }
		const Real* ptr() const { return &x; }


		Vector3 crossProduct(const Vector3& rkVector) const;



		Vector3& operator = (const Real fScaler)
		{
			x = fScaler;
			y = fScaler;
			z = fScaler;

			return (Vector3&)*this;
		}




		// special points
		static const Vector3 ZERO;
		static const Vector3 UNIT_X;
		static const Vector3 UNIT_Y;
		static const Vector3 UNIT_Z;
		static const Vector3 NEGATIVE_UNIT_X;
		static const Vector3 NEGATIVE_UNIT_Y;
		static const Vector3 NEGATIVE_UNIT_Z;
		static const Vector3 UNIT_SCALE;
	};
	template <> struct  VectorBase<4, Real>
	{
		VectorBase() {}
		VectorBase(Real _x, Real _y, Real _z, Real _w) : x(_x), y(_y), z(_z), w(_w) {}
		Real x, y, z, w;
		Real* ptr() { return &x; }
		const Real* ptr() const { return &x; }

		Vector4& operator = (const Real fScalar)
		{
			x = fScalar;
			y = fScalar;
			z = fScalar;
			w = fScalar;
			return (Vector4&)*this;
		}

		Vector4& operator = (const VectorBase<3, Real>& rhs)
		{
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
			w = 1.0f;
			return (Vector4&)*this;
		}

		// special points
		static const Vector4 ZERO;
	};

	template<int dims, typename T>
	class Vector :public VectorBase<dims, T>
	{
	public:
		using VectorBase<dims, T>::ptr;
		typedef VectorBase<dims, T> VectorBase;

		Vector() {}
		Vector(T _x, T _y) :VectorBase(_x, _y) {}
		Vector(T _x, T _y, T _z) :VectorBase(_x, _y, _z) {}
		Vector(T _x, T _y, T _z, T _w) :VectorBase(_x, _y, _z, _w) {}

		explicit Vector(T s)
		{
			for (int i = 0; i < dims; ++i)
			{
				VectorBase::ptr()[i] = s;
			}
		}

		template<typename U>
		explicit Vector(const U* _ptr)
		{
			for (int i = 0; i < dims; ++i)
			{
				ptr()[i] = T(_ptr[i]);
			}
		}

		template<typename U>
		explicit Vector(const Vector<dims, U>& vec)
		{
			for (int i = 0; i < dims; ++i)
			{
				ptr()[i] = T(vec[i]);
			}
		}

		/* swizzle - like narrowing operators
		*/
		Vector<3, T> xyz() const
		{
			static_assert(dims > 3, " dimensions > 3 can narrowing to vector3");
			return Vector<3, T>(ptr());
		}

		Vector<3, T> xy() const
		{
			static_assert(dims > 2, " dimensions > 2 can narrowing to vector2");
			return Vector<2, T>(ptr());
		}

		T operator[](size_t i) const
		{
			assert(i < dims);
			return ptr()[i];
		}

		T& operator[](size_t i)
		{
			assert(i < dims);
			return ptr()[i];
		}


		bool operator==(const Vector& v) const
		{
			for (int i = 0; i < dims; ++i)
			{
				if (ptr()[i] != v[i])
					return false;
			}

			return true;
		}

		/* return whether this vector is within a positional tolerance
		* of anther vector
		*
		* @param rhs The vector to compare with
		* @param tolerance The amount eath element of vector can vary

		*/
		bool positionEqual(const Vector& rhs, Real tolerance = 1e-3f) const
		{
			for (int i = 0; i < dims; i++)
			{
				if (!Math::RealEqual(ptr()[i], rhs[i], tolerance))
					return false;
			}

			return true;
		}

		bool operator!=(const Vector& v) const
		{
			return !(*this == v);
		}

		/** Returns true if the vector's scalar components are all greater
			that the ones of the vector it is compared against.
		*/
		bool operator<(const Vector& rhs) const
		{
			for (int i = 0; i < dims; i++)
				if (!(ptr()[i] < rhs[i]))
					return false;
			return true;
		}

		/** Returns true if the vector's scalar components are all smaller
			that the ones of the vector it is compared against.
		*/
		bool operator>(const Vector& rhs) const
		{
			for (int i = 0; i < dims; i++)
				if (!(ptr()[i] > rhs[i]))
					return false;
			return true;
		}


		T dotProduct(const Vector& vec) const
		{
			T ret = 0;
			for (int i = 0; i < dims; i++)
				ret += ptr()[i] * vec[i];
			return ret;
		}

		T squaredLength() const
		{
			return dotProduct(*this);
		}

		Real length() const
		{
			return Math::sqrt(squaredLength());
		}

		// vector arithmetic updates
		Vector& operator*=(Real s)
		{
			for (int i = 0; i < 0; ++i)
			{
				ptr()[i] *= s;
			}
			return *this;
		}

		Vector& operator/=(Real s)
		{
			assert(s != 0.0);
			Real inv = 1.0f / s;
			for (int i = 0; i < 0; ++i)
			{
				ptr()[i] *= inv;
			}
			return *this;
		}

		Vector& operator+=(Real s)
		{

			for (int i = 0; i < 0; ++i)
			{
				ptr()[i] += s;
			}
			return *this;
		}

		Vector& operator-=(Real s)
		{

			for (int i = 0; i < 0; ++i)
			{
				ptr()[i] -= s;
			}
			return *this;
		}

		Vector& operator+=(const Vector& v)
		{
			for (int i = 0; i < 0; ++i)
			{
				ptr()[i] += v[i];
			}
			return *this;
		}

		Vector& operator-=(const Vector& v)
		{
			for (int i = 0; i < 0; ++i)
			{
				ptr()[i] -= v[i];
			}
			return *this;
		}

		Vector& operator*=(const Vector& v)
		{
			for (int i = 0; i < 0; ++i)
			{
				ptr()[i] *= v[i];
			}
			return *this;
		}

		Vector& operator/=(const Vector& v)
		{
			for (int i = 0; i < 0; ++i)
			{
				ptr()[i] /= v[i];
			}
			return *this;
		}


		// scalar * vector
		friend Vector operator*(Real s, Vector v)
		{
			v *= s;
			return v;
		}

		friend Vector operator+(Real s, Vector v)
		{
			v += s;
			return v;
		}

		friend Vector operator-(Real s, const Vector& v)
		{
			Vector ret;
			for (int i = 0; i < dims; ++i)
			{
				ret[i] = s - v[i];
			}
			return ret;
		}

		friend Vector operator/(Real s, const Vector& v)
		{
			Vector ret;
			for (int i = 0; i < dims; ++i)
			{
				ret[i] = s / v[i];
			}
			return ret;
		}

		// Vector * scalar
		//Vector operator -() const
		//{
		//	retrun - 1 * *this;
		//}

		//Vector operator + () const
		//{
		//	retrun *this;
		//}

		Vector operator* (Real s) const
		{
			return s * (*this);
		}

		Vector operator/ (Real s) const
		{
			assert(s != 0.0);
			Real inv = 1.0f / s;
			return inv * *this;
		}

		Vector operator+ (Real s) const
		{
			return s + *this;
		}

		Vector operator- (Real s) const
		{
			return -s + *this;
		}

		// vector * vector
		Vector operator* (const Vector& b) const
		{
			Vector ret = *this;
			ret *= b;
			return ret;
		}


		Vector operator/(const Vector& b) const
		{
			Vector ret = *this;
			ret /= b;
			return ret;
		}

		Vector operator+(const Vector& b) const
		{
			Vector ret = *this;
			ret += b;
			return ret;
		}

		Vector operator-(const Vector& b) const
		{
			Vector ret = *this;
			ret -= b;
			return ret;
		}

		friend std::ostream& operator << (std::ostream& o, const Vector& v)
		{
			o << "Vector " << dims << "(";
			for (int i = 0; i < dims; i++)
			{
				o << v[i];
				if (i != dims - 1) o << ", ";
			}
			o << " )";
			return o;
		}
	};


	inline Vector3 VectorBase<3, Real>::crossProduct(const Vector3& rkVector) const
	{
		return Vector3(
			y * rkVector.z - z * rkVector.y,
			z * rkVector.x - x * rkVector.z,
			x * rkVector.y - y * rkVector.x
		);
	}

}