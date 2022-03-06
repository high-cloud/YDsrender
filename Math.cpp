#include"YDPrerequisites.h"
#include "Math.h"
#include"Vector.h"


namespace YDSR
{
	const Real Math::Pi = 3.1415926;

	// barycentric method 
	std::tuple<float,float,float> YDSR::Math::barycentric(Vector2 a, Vector2 b, Vector2 c, Vector2 pos)
	{

		Vector2 ac = a-c, ab = a - b;
		float gamma = (pos.crossProduct(ab)+ a.crossProduct(b))
			/ (c.crossProduct(ab) + a.crossProduct(b) );

		float beta = (pos.crossProduct(ac) + a.crossProduct(c))
			/ (b.crossProduct(ac) + a.crossProduct(c));

		return { 1.0f - gamma - beta, beta, gamma };
	}

}