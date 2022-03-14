#pragma once

#define NOMINMAX

namespace YDSR
{
	typedef float Real;
	typedef float Radian;
	typedef float Dgree;

	template<int dims, typename T> class Vector;
	typedef Vector<2, Real> Vector2;
	typedef Vector<3, Real> Vector3;
	typedef Vector<4, Real> Vector4;
}