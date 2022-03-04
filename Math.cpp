#include"YDPrerequisites.h"
#include "Math.h"
#include"Vector.h"


namespace YDSR
{

	// barycentric method 
	std::tuple<float,float,float> YDSR::Math::barycentric(Vector2 a, Vector2 b, Vector2 c, Vector2 pos)
	{
		Vector2 ac = a - c, ab = a - b;
		float gamma = (pos.crossProduct(a - b)+ a.crossProduct(b))
			/ (c.crossProduct(a-b) + a.crossProduct(b) );

		float beta = (pos.crossProduct(a - c) + a.crossProduct(c))
			/ (b.crossProduct(a - c) + a.crossProduct(c));

		return { 1.0f - gamma - beta, beta, gamma };
	}

}