#pragma once

#include<limits>
#include<cmath>
#include<tuple>
#include"YDPrerequisites.h"



namespace YDSR
{

	class Math
	{
	public:
		static const Real Pi;
		static bool RealEqual(Real a, Real b,
			Real tolerance = std::numeric_limits<Real>::epsilon())
		{
			return std::abs(b - a) <= tolerance;
		}

		static Real clamp(Real val, Real low, Real high)
		{
			if (low > val)
				return low;

			if (high < val)
				return high;

			return val;
		}

		static Real sqrt(Real num)
		{
			return std::sqrt(num);
		}

		static std::tuple<float, float, float> barycentric(Vector2 a, Vector2 b, Vector2 c, Vector2);

		static Real tan(Radian theta)
		{
			return std::tan(theta);
		}

		static Real real_min()
		{
			return std::numeric_limits<Real>::min();
		}

		static Real real_max()
		{
			return std::numeric_limits<Real>::min();
		}
	};



}