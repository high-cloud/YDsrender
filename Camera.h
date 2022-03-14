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

		Matrix4 vp;  // project * view

		Matrix4 view;

		Matrix4 projection;

	public:
		Camera() :pos{}, gazeAt{}, up{}{}
		Camera(Vector3 _pos, Vector3 _gazeAt, Vector3 _up) :pos(_pos), gazeAt(_gazeAt),up(_up) {}

		void setPos(const Vector3& _pos)
		{
			pos = _pos;
			callMatrix();
		}

		void move(const Vector3& step)
		{
			setPos(pos + step);
		}


		// fov in degree
		void setPerspectiveForLH(Dgree _fov, Real _aspect, float _nearZ, float _farZ)
		{
			fov = _fov/180.f*Math::Pi;
			aspect = _aspect;
			nearZ = _nearZ;
			farZ = _farZ;
			callMatrix();
		}

		const Matrix4& getViewMatrix() const
		{
			return view;
		}

		const Matrix4& getVPMatrix() const
		{
			return vp;
		}

		const Matrix4& getProjectMatrix() const
		{
			return projection;
		}

	private:
		void callMatrix();
	};


}