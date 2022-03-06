#pragma once

#include"Vector.h"
#include"YDPrerequisites.h"
#include"Matrix4.h"

namespace YDSR
{

	class Camera
	{
	private:
		Vector3 pos;
		Vector3 gazeAt;
		Vector3 up;

		Radian fov{};
		Real aspect{};
		Real nearZ{};
		Real farZ{};

		Matrix4 worldToCamera;

	public:
		Camera(Vector3 _pos, Vector3 _gazeAt, Vector3 _up) :pos(_pos), gazeAt(_gazeAt),up(_up) {}

		void setPos(const Vector3& _pos)
		{
			pos = _pos;

		}

		// fov in degree
		void setPerspectiveForLH(Dgree _fov, Real _aspect, float _nearZ, float _farZ)
		{
			fov = _fov/180.f*Math::Pi;
			aspect = aspect;
			nearZ = _nearZ;
			farZ = _farZ;
		}

	private:
		void calMatrix();
	};


}