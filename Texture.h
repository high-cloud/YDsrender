#pragma once

#include"YDPrerequisites.h"
#include<memory>
//#include<string>
#include<windows.h>

namespace YDSR {

class Texture
{

	size_t width;
	size_t height;

	std::shared_ptr<Vector3[] > pixels;

public:
	Texture() :width{}, height{}, pixels{}{}

	Texture(const WCHAR* path);
	void loadImage(const WCHAR* path);

	Vector3 sample(Real u, Real v) const;

	Vector3 getPixel(int x, int y) const;

};


}