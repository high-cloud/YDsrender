#pragma once
#include<array>
#include"YDPrerequisites.h"
#include"Vector.h"
namespace YDSR
{

class Triangle
{
public:
	Vector4 v[3]; // original coordinates of trangle

	Vector3 color[3]; // color at eath vertex

	Vector2 tex_coords[3]; // uv

	Vector3 normal[3]; // normal vector at each vertex


	Triangle() {}

	void setVertex(int idx, Vector4 vec);
	void setNormal(int idx, Vector3 n);
	void setColor(int idx, float r, float g, float b);

	void setNormals(const std::array<Vector3, 3>& normals);
	void setColors(const std::array< Vector3, 3>& colors);
	void setTexCoord(int idx, Vector2 uv);

	Vector3 getColor(Real a,Real b,Real c) const
	{
		return a*color[0]+b*color[1]+c*color[2];
	}

	std::array<Vector4, 3> toVector4() const;

};


}