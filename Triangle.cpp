#include "Triangle.h"

namespace YDSR
{

	void YDSR::Triangle::setVertex(int idx, Vector4 vec)
	{
		v[idx] = vec;
	}

	void Triangle::setColor(int idx, float r, float g, float b)
	{
		color[idx] = Vector3(r, g, b);
	}

	std::array<Vector4, 3> Triangle::toVector4() const
	{
		return std::array<Vector4, 3>{v[0], v[1], v[2]};
	}

}