#include "pch.h"
#include "Sprite.h"

Sprite::Sprite(CString src)
{
	img.Load(src);
}

void Sprite::draw(CDC& dc, int x, int y)
{
	CDC* im = CDC::FromHandle(img.GetDC());
	dc.BitBlt(x, y, img.GetWidth(), img.GetHeight(), im, 0, 0, SRCCOPY);

	img.ReleaseDC();
}
