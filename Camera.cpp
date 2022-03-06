#include "Camera.h"

void YDSR::Camera::calMatrix()
{
	Matrix4 trans(1, 0, 0, -pos[0],
		0,1,0,-pos[1],
		0,0,1,-pos[2],
		0,0,0,1);


	Vector3 tmp = gazeAt.crossProduct(up); // the x axis of camera

	Matrix4 rotation(tmp.x, tmp.y, tmp.z, 0,
		up.x, up.y, up.z, 0,
		-gazeAt.x, -gazeAt.y, -gazeAt.z, 0,
		0, 0, 0, 1);

	Real nmf_inv = 1.0f / (nearZ - farZ);
	Real tan_inv = 1.0f / Math::tan(fov * 0.5f);
	Matrix4 projection(tan_inv, 0, 0, 0,
		0, tan_inv/aspect, 0, 0,
		0, 0, 2*(nearZ +farZ)* nmf_inv, -(2*nearZ * farZ +nearZ+farZ)* nmf_inv,
		0, 0, 0, 1);

	worldToCamera = projection * rotation * trans;
}
