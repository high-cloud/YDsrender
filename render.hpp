#pragma once


#define NOMINMAX

#include<memory>
#include<algorithm>
#include"framework.h"
#include"YDPrerequisites.h"
#include"Triangle.h"
#include"Math.h"


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



	void drawTriangle(const Triangle& tri);
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

	clearBuffer();
}

void YDSR::update(HWND hWnd)
{
	clearBuffer();


	// test traigle
	Triangle tri1;
	tri1.setVertex(0, { 34,45,1,1 });
	tri1.setVertex(1, { 200,45,1,1 });
	tri1.setVertex(2, { 34,200,1,1 });

	tri1.setColor(0, 0.5, 0.5, 0.5);
	drawTriangle(tri1);



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

			g_depthBuff[idx] = std::numeric_limits<Real>::min();
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

	g_frameBuff[(g_height-y) * g_width + x] = color;
}

void YDSR::drawPixel(int x, int y, Vector3 color)
{
	unsigned int uintColor = static_cast<unsigned int>(color.x * 255) << 16
		+ static_cast<unsigned int>(color.x * 255) << 8
		+ static_cast<unsigned int>(color.x * 255);
	drawPixel(x, y, uintColor);
}

void YDSR::drawTriangle(const Triangle& tri)
{
	auto v = tri.toVector4();
	// find box
	Real min_x = std::floor(std::min(v[0].x, std::min(v[1].x, v[2].x)));
	Real max_x = std::ceil(std::max(v[0].x, std::max(v[1].x, v[2].x)));
	Real min_y = std::floor(std::min(v[0].y, std::min(v[1].y, v[2].y)));
	Real max_y = std::ceil(std::max(v[0].y, std::max(v[1].y, v[2].y)));

	// iterate in box
	for (int j = min_y; j < max_y; ++j)
		for (int i = min_x; i <= max_x; ++i)
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
					auto color = tri.getColor();

					drawPixel(i, j, color);
				}
			}



		}
}
