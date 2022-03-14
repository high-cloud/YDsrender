#include "Texture.h"
#include"Vector.h"
#include<Windows.h>

YDSR::Texture::Texture(const WCHAR* path) :height{}, width{}, pixels{}
{
    loadImage(path);
}

void YDSR::Texture::loadImage(const WCHAR* path)
{

    HBITMAP hbitmap = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    HDC hdc = CreateCompatibleDC(NULL);
    SelectObject(hdc, hbitmap);

    BITMAP bmp;
    GetObject(hbitmap, sizeof(BITMAP), (LPBYTE)&bmp);

    // set height and weight
    height = bmp.bmHeight;
    width = bmp.bmWidth;

    pixels.reset(new Vector3[height * width]);


    auto ucolor2real = [](int color) {return (Real)color / 256; };


    for(int i=0;i<width;++i)
        for (int j=0; j < height; ++j)
        {
            auto color = GetPixel(hdc, i, j);
            
            auto temp = Vector3(ucolor2real(color % 256), ucolor2real((color >> 8) % 256), ucolor2real((color >> 16) % 256));
            pixels[(height-j-1) * width + i] = Vector3(ucolor2real(color % 256), ucolor2real((color >> 8) % 256), ucolor2real((color >> 16) % 256));

        }

}

YDSR::Vector3 YDSR::Texture::sample(Real u, Real v) const
{
    assert(u <= 1.0f && v <= 1.0f && u >= 0.0f && v >= 0.0f);

    int x = round( u * (width-1));
    int y = round(v * (height-1));

    return pixels[y * width + x];
}

YDSR::Vector3 YDSR::Texture::getPixel(int x, int y) const
{
    return pixels[x+y*width];
}
