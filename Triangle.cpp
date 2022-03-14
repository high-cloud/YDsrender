#include "Triangle.h"

namespace YDSR
{

	void YDSR::Triangle::setVertex(int idx, Vector4 vec)
	{
		v[idx] = vec;
	}

	void Triangle::setNormal(int idx, Vector3 n)
	{
		normal[idx] = n;
	}

	void Triangle::setColor(int idx, float r, float g, float b)
	{
		color[idx] = Vector3(r, g, b);
	}

	void Triangle::setTexCoord(int idx, Vector2 uv)
	{
		tex_coords[idx] = uv;
	}

	std::array<Vector4, 3> Triangle::toVector4() const
	{
		return std::array<Vector4, 3>{v[0], v[1], v[2]};
	}

}