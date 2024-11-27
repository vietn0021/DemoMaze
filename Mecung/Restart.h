#pragma once
#include <afxwin.h>
#include "Sprite.h"

class Restart {
public:
    CRect rect;         // Vùng chữ nhật của nút
    CString label;      // Nhãn hiển thị trên nút
    Sprite res;

    // Constructor
    Restart(int x, int y, int width, int height, const CString& text);

    // Vẽ nút
    void draw(CDC& dc);

    // Kiểm tra nếu nút được nhấn
    bool isClicked(CPoint point);
};
