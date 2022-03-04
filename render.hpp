#pragma once
#include<memory>
#include"framework.h"

// used to render 
namespace YDSR
{
	int g_width;
	int g_height;

	HDC g_tempDC;
	HBITMAP g_tempBm;
	HBITMAP g_oldBm;
	unsigned int* g_frameBuff;
	std::shared_ptr<float[]> g_depthBuff = nullptr;

	unsigned int bgColor = (123 << 16) | (195 << 8) | 221;

	// init render width height
	void initRenderer(int w, int h, HWND hWnd);

	// every frame draw
	void update(HWND hWnd);

	// clear buffer
	void clearBuffer();

	void shutDown();

	void drawPixel(int x, int y,unsigned int color);
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
		(DWORD)w*h*4,0,0,0,0}};
	g_tempBm = CreateDIBSection(g_tempDC, &bi, DIB_RGB_COLORS, (void**)&g_frameBuff, NULL, 0);
	g_oldBm = (HBITMAP)SelectObject(g_tempDC, g_tempBm);
	g_depthBuff.reset(new float[w * h]);

	clearBuffer();
}

void YDSR::update(HWND hWnd)
{
	clearBuffer();

	//present buffer to screen
	HDC hdc = GetDC(hWnd);
	BitBlt(hdc, 0, 0, g_width, g_height,g_tempDC, 0, 0, SRCCOPY);
	ReleaseDC(hWnd, hdc);
}

void YDSR::clearBuffer()
{
	for(int row=0;row<g_height;++row)
		for (int col = 0; col < g_width; ++col)
		{
			int idx = row * g_width + col;
			g_frameBuff[idx] = bgColor;

			g_depthBuff[idx] = 1.0f;
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

	g_frameBuff[y * g_width + x] = color;
}