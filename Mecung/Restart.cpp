#include "pch.h"
#include "Restart.h"
// Constructor
Restart::Restart(int x, int y, int width, int height, const CString& text)
    : rect(x, y, x + width, y + height), label(text), res(_T("res/restart.jpg")) {}

// Vẽ nút
void Restart::draw(CDC& dc)  {
    // Vẽ khung nút
    dc.Rectangle(&rect);

    /// Tô nền nút
    dc.FillSolidRect(&rect, RGB(200, 200, 200));  // Màu xám nhạt

    //// Vẽ nhãn nút
    dc.SetTextColor(RGB(0, 0, 0));  // Màu đen
    dc.SetBkMode(TRANSPARENT);
    dc.DrawText(label, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
   /* res.draw(dc, 1200, 300);*/
}

// Kiểm tra nếu nút được nhấn
bool Restart::isClicked(CPoint point) {
    return rect.PtInRect(point);
}