#include"Vector.h"


namespace YDSR
{
	const Vector2 VectorBase<2, Real>::ZERO(0);
	const Vector2 VectorBase<2, Real>::UNIT_X(1, 0);
	const Vector2 VectorBase<2, Real>::UNIT_Y(0, 1);
	const Vector2 VectorBase<2, Real>::NEGATIVE_UNIT_X(-1, 0);
	const Vector2 VectorBase<2, Real>::NEGATIVE_UNIT_Y(0, -1);
	const Vector2 VectorBase<2, Real>::UNIT_SCALE(1);

	const Vector3 VectorBase<3, Real>::ZERO(0);
	const Vector3 VectorBase<3, Real>::UNIT_X(1, 0, 0);
	const Vector3 VectorBase<3, Real>::UNIT_Y(0, 1, 0);
	const Vector3 VectorBase<3, Real>::UNIT_Z(0, 0, 1);
	const Vector3 VectorBase<3, Real>::NEGATIVE_UNIT_X(-1, 0, 0);
	const Vector3 VectorBase<3, Real>::NEGATIVE_UNIT_Y(0, -1, 0);
	const Vector3 VectorBase<3, Real>::NEGATIVE_UNIT_Z(0, 0, -1);
	const Vector3 VectorBase<3, Real>::UNIT_SCALE(1);

	const Vector4 VectorBase<4, Real>::ZERO(0);
}