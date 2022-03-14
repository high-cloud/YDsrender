#include "Model.h"
#include"OBJ_loader.h"

bool YDSR::Model::loadMesh(std::string path)
{
	objl::Loader loader;

	if (!loader.LoadFile(path))
		return false;

	for (auto &mesh : loader.LoadedMeshes)
	{
		for (int i = 0; i < mesh.Vertices.size(); i += 3)
		{
			Triangle* t = new Triangle();
			for (int j = 0; j < 3; ++j)
			{
				t->setVertex(j, Vector4(mesh.Vertices[i + j].Position.X, mesh.Vertices[i + j].Position.Y, mesh.Vertices[i + j].Position.Z, 1.0));
				t->setNormal(j, Vector3(mesh.Vertices[i + j].Normal.X, mesh.Vertices[i + j].Normal.Y, mesh.Vertices[i + j].Normal.Z));
				t->setTexCoord(j, Vector2(mesh.Vertices[i + j].TextureCoordinate.X, mesh.Vertices[i + j].TextureCoordinate.Y));
			}
			triangleList.push_back(t);
		}
	}

	return true;
}

bool YDSR::Model::loadDiffuseMap(const WCHAR* path)
{
	diffuse_map.loadImage(path);
	return true;
}
