#pragma once

#include"YDPrerequisites.h"
#include"Triangle.h"
#include<vector>
#include"Texture.h"


namespace YDSR {


	class Model
	{
	public:
		std::vector<Triangle*> triangleList;
		
		Texture diffuse_map;

	public:
		bool loadMesh(std::string path);
		bool loadDiffuseMap(const WCHAR* path);
	};


}