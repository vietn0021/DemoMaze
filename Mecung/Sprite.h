#pragma once
class Sprite
{
public:
	Sprite(CString src);

	CImage img;

	void draw(CDC &dc, int x, int y);
};

