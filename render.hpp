#pragma once


#define NOMINMAX

#include<memory>
#include<algorithm>
#include<vector>

#include"framework.h"
#include"YDPrerequisites.h"
#include"Triangle.h"
#include"Math.h"
#include"Camera.h"
#include"Model.h"

// used to render 
namespace YDSR
{
	int g_width;
	int g_height;

	HDC g_tempDC;
	HBITMAP g_tempBm;
	HBITMAP g_oldBm;
	unsigned int* g_frameBuff;
	std::shared_ptr<Real[]> g_depthBuff = nullptr;

	// camera
	Camera g_camera;

	unsigned int bgColor = (123 << 16) | (195 << 8) | 221;

	// init render width height
	void initRenderer(int w, int h, HWND hWnd);

	// every frame draw
	void update(HWND hWnd);

	// clear buffer
	void clearBuffer();

	void shutDown();

	void drawPixel(int x, int y, unsigned int color);
	void drawPixel(int x, int y, Vector3 color);


	void draw(std::vector<Triangle*>& triangleList);
	void draw(const Model& model);
	void drawTriangle(const Triangle& tri);

	// helper function
	Vector4 to_vec4(const Vector3& v3, Real w);


	Model model;


	void onKeyBoardMessage(UINT message, WPARAM wParam, LPARAM lParam);
}

void YDSR::initRenderer(int w, int h, HWND hWnd)
{
	g_width = w;
	g_height = h;

	// create a window buffer
	HDC hDC = GetDC(hWnd);
	g_tempDC = CreateCompatibleDC(hDC);
	ReleaseDC(hWnd, hDC);

	// create a bitmap buffer for dc
	BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER),w,-h,1,32,BI_RGB,
		(DWORD)w * h * 4,0,0,0,0} };
	g_tempBm = CreateDIBSection(g_tempDC, &bi, DIB_RGB_COLORS, (void**)&g_frameBuff, NULL, 0);
	g_oldBm = (HBITMAP)SelectObject(g_tempDC, g_tempBm);
	g_depthBuff.reset(new Real[w * h]);

	// init camera
	g_camera = Camera({ 0.0f,0.0f,2.0f }, { 0.0f,0.0f,-1.0f }, { 0.0f,1.0f,0.0f });
	g_camera.setPerspectiveForLH(45.0f, 2.0f, -1.0f, -700.0f);


	// init model
	if (!model.loadMesh("./models/diablo3_pose.obj"))
	{
		printf("error in loadmesh");
	}
	model.loadDiffuseMap(L"./models/diablo3_pose_diffuse.bmp");


	clearBuffer();
}

void YDSR::update(HWND hWnd)
{
	clearBuffer();


	// test traigle
	Triangle tri1;
	tri1.setVertex(0, { 50,50,-10,1 });
	tri1.setVertex(1, { 100,50,-10,1 });
	tri1.setVertex(2, { 50,100,-10,1 });

	tri1.setColor(0, 0.5, 0.5, 0.5);
	//drawTriangle(tri1);

	std::vector<Triangle*> triList;
	triList.push_back(&tri1);

	//draw(triList);
	draw(model);

	// render



	//present buffer to screen
	HDC hdc = GetDC(hWnd);
	BitBlt(hdc, 0, 0, g_width, g_height, g_tempDC, 0, 0, SRCCOPY);
	ReleaseDC(hWnd, hdc);
}

void YDSR::clearBuffer()
{
	for (int row = 0; row < g_height; ++row)
		for (int col = 0; col < g_width; ++col)
		{
			int idx = row * g_width + col;
			g_frameBuff[idx] = bgColor;

			g_depthBuff[idx] = -std::numeric_limits<Real>::max();
		}
}

void YDSR::shutDown()
{
	if (g_tempDC)
	{
		if (g_oldBm)
		{
			SelectObject(g_tempDC, g_oldBm);
			g_oldBm = nullptr;
		}
		DeleteDC(g_tempDC);
		g_tempDC = nullptr;
	}

	if (g_tempBm)
	{
		DeleteObject(g_tempBm);
		g_tempBm = nullptr;
	}
}

void YDSR::drawPixel(int x, int y, unsigned int color)
{
	if (x < 0 || x >= g_width || y < 0 || y >= g_height)
		return;

	g_frameBuff[(g_height - y-1) * g_width + x] = color;
}

void YDSR::drawPixel(int x, int y, Vector3 color)
{
	unsigned int uintColor = static_cast<unsigned int>(color.x * 255) << 16
		+ static_cast<unsigned int>(color.x * 255) << 8
		+ static_cast<unsigned int>(color.x * 255);
	drawPixel(x, y, uintColor);
}

void YDSR::draw(std::vector<Triangle*>& triangleList)
{

	// transfer to world space
	// todo: add Model with model matrix

	// transfer to view space


	for (const auto& t : triangleList)
	{
		Triangle newtri = *t;

		// transfer to Homogeneous Clip Space
		Vector4 v[] = {
			g_camera.getVPMatrix() * t->v[0],
			g_camera.getVPMatrix() * t->v[1],
			g_camera.getVPMatrix() * t->v[2]
		};

		// clip and reject
		// todo

		// transfer to ndc space ( divide w )
		for (int i = 0; i < 3; ++i)
		{
			v[i].x /= v[i].w;
			v[i].y /= v[i].w;
			v[i].z /= v[i].w;
		}

		// transfer to view space
		Real f1 = (50 - 0.1) / 2.0;
		Real f2 = (50 + 0.1) / 2.0;

		for (auto& vert : v)
		{
			vert.x = 0.5 * g_width * (vert.x + 1.0f);
			vert.y = 0.5 * g_height * (vert.y + 1.0f);
			//vert.z = vert.z ;
		}

		// transfer normal vector
		Matrix4 inv_mv = g_camera.getViewMatrix().inverse().transpose();
		Vector4 n[] = {
			inv_mv * to_vec4(t->normal[0],0.0f),
			inv_mv * to_vec4(t->normal[1],0.0f),
			inv_mv * to_vec4(t->normal[2],0.0f)
		};

		for (int i = 0; i < 3; ++i)
		{
			newtri.setVertex(i, v[i]);
			newtri.setNormal(i, n[i].xyz());
		}

		drawTriangle(newtri);


	}



	// transfer to screen space 

	// rasterize 
}

void YDSR::draw(const Model& model)
{
	auto &triangleList = model.triangleList;
	// transfer to world space
	// todo: add Model with model matrix

	// transfer to view space


	for (const auto& t : triangleList)
	{
		Triangle newtri = *t;

		// transfer to Homogeneous Clip Space
		Vector4 v[] = {
			g_camera.getVPMatrix() * t->v[0],
			g_camera.getVPMatrix() * t->v[1],
			g_camera.getVPMatrix() * t->v[2]
		};

		// clip and reject
		// todo

		// transfer to ndc space ( divide w )
		for (int i = 0; i < 3; ++i)
		{
			v[i].x /= v[i].w;
			v[i].y /= v[i].w;
			v[i].z /= v[i].w;
		}

		// transfer to view space
		Real f1 = (50 - 0.1) / 2.0;
		Real f2 = (50 + 0.1) / 2.0;

		for (auto& vert : v)
		{
			vert.x = 0.5 * g_width * (vert.x + 1.0f);
			vert.y = 0.5 * g_height * (vert.y + 1.0f);
			//vert.z = vert.z ;
		}

		// transfer normal vector
		Matrix4 inv_mv = g_camera.getViewMatrix().inverse().transpose();
		Vector4 n[] = {
			inv_mv * to_vec4(t->normal[0],0.0f),
			inv_mv * to_vec4(t->normal[1],0.0f),
			inv_mv * to_vec4(t->normal[2],0.0f)
		};

		for (int i = 0; i < 3; ++i)
		{
			newtri.setVertex(i, v[i]);
			newtri.setNormal(i, n[i].xyz());
		}

		// add diffuse color
		for (int i = 0; i < 3; ++i)
		{
			auto color = model.diffuse_map.sample(newtri.tex_coords[i].x, newtri.tex_coords[i].y);
			newtri.setColor(i, color.x,color.y,color.z);
		}

		drawTriangle(newtri);


	}



	// transfer to screen space 

	// rasterize 
}

void YDSR::drawTriangle(const Triangle& tri)
{
	auto v = tri.toVector4();
	// find box
	Real min_x = std::floor(std::min(v[0].x, std::min(v[1].x, v[2].x)));
	Real max_x = std::ceil(std::max(v[0].x, std::max(v[1].x, v[2].x)));
	Real min_y = std::floor(std::min(v[0].y, std::min(v[1].y, v[2].y)));
	Real max_y = std::ceil(std::max(v[0].y, std::max(v[1].y, v[2].y)));


	min_x = Math::clamp(min_x, 0, g_width);
	max_x = Math::clamp(max_x, 0, g_width);
	min_y = Math::clamp(min_y, 0, g_height);
	max_y = Math::clamp(max_y, 0, g_height);
	// iterate in box
	for (int j = min_y; j < max_y; ++j)
		for (int i = min_x; i < max_x; ++i)
		{

			auto [alpha, beta, gamma] = Math::barycentric(v[0].xy(), v[1].xy(), v[2].xy(), Vector2(i + 0.5f, j + 0.5f));

			// judge if in tri
			if (alpha >= 0.0f && beta >= 0.0f && gamma >= 0.0f)
			{
				auto w_resiprocal = 1.0 / (alpha / v[0].w + beta / v[1].w + gamma / v[2].w);
				auto z_interpolated = alpha * v[0].z / v[0].w + beta * v[1].z / v[1].w + gamma * v[2].z / v[2].w;
				z_interpolated *= w_resiprocal;

				// judge whether closer to camera
				if (g_depthBuff[j * g_width + i] < z_interpolated)
				{
					g_depthBuff[j * g_width + i] = z_interpolated;
					auto color = tri.getColor(alpha,beta,gamma);

					drawPixel(i, j, color);
				}
			}



		}
}

YDSR::Vector4 YDSR::to_vec4(const Vector3& v3, Real w)
{
	return Vector4(v3.x, v3.y, v3.z, w);
}

//std::vector<YDSR::Triangle*> YDSR::loadMesh(std::string path)
//{
//	std::vector<Triangle*> tris;
//
//	objl::Loader loader;
//
//	if (!loader.LoadFile(path))
//		return tris;
//
//	for (auto &mesh : loader.LoadedMeshes)
//	{
//		for (int i = 0; i < mesh.Vertices.size(); i += 3)
//		{
//			Triangle* t = new Triangle();
//			for (int j = 0; j < 3; ++j)
//			{
//				t->setVertex(j, Vector4(mesh.Vertices[i + j].Position.X, mesh.Vertices[i + j].Position.Y, mesh.Vertices[i + j].Position.Z, 1.0));
//				t->setNormal(j, Vector3(mesh.Vertices[i + j].Normal.X, mesh.Vertices[i + j].Normal.Y, mesh.Vertices[i + j].Normal.Z));
//				t->setTexCoord(j, Vector2(mesh.Vertices[i + j].TextureCoordinate.X, mesh.Vertices[i + j].TextureCoordinate.Y));
//			}
//			tris.push_back(t);
//		}
//	}
//
//	return tris;
//}

void YDSR::onKeyBoardMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	Real moveSpeed = 0.1f;
	switch (wParam)
	{
	case 'a':
	case 'A':
		g_camera.move(moveSpeed*Vector3( - 1.0, 0.0, 0.0 ));
		break;
	case 'd':
	case 'D':
		g_camera.move(moveSpeed*  Vector3(1.0, 0.0, 0.0));
		break;
	case 's':
	case 'S':
		g_camera.move(moveSpeed*Vector3( 0.0,0.0,1.0 ));
		break;
	case 'W':
	case 'w':
		g_camera.move(moveSpeed*Vector3( 0.0,0.0,-1.0 ));
		break;

	default:
		break;
	}
}
