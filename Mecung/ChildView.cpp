#include "pch.h"
#include "framework.h"
#include "Mecung.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildView

// Constructor của CChildView
CChildView::CChildView() : maze(20, 20), cellSize(20), restartButton(maze.width* cellSize + 300, 10, 100, 40, _T("Restart"))
{
    maze.generate();  // Sinh mê cung khi bắt đầu
    remainingTime = 900;
    timerID = 0;
}

CChildView::~CChildView()
{
    if (timerID != 0)
        KillTimer(timerID);
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
    ON_WM_PAINT()
    ON_WM_KEYDOWN()
    ON_WM_SIZE()
    ON_WM_LBUTTONDOWN()
    ON_WM_TIMER()
    ON_WM_CREATE()
END_MESSAGE_MAP()

// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
    if (!CWnd::PreCreateWindow(cs))
        return FALSE;

    cs.dwExStyle |= WS_EX_CLIENTEDGE;
    cs.style &= ~WS_BORDER;
    cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
        ::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1), nullptr);
    
    return TRUE;
}



void CChildView::OnPaint()
{
    CPaintDC dc(this);  // device context for painting
    CRect clientRect;
    GetClientRect(&clientRect);


    //// Kích thước mỗi ô trong mê cung
    int cellSize = min(clientRect.Width() / maze.width, clientRect.Height() / maze.height);

   /// Tô nền màu xanh lá cây nhạt cho từng ô
    for (int y = 0; y < maze.height; ++y) {
        for (int x = 0; x < maze.width; ++x) {
            int left = x * cellSize;
            int top = y * cellSize;
            int right = left + cellSize;
            int bottom = top + cellSize;

    //        // Tô màu nền
           dc.FillSolidRect(left, top, cellSize, cellSize, RGB(153,255,204));  
       }
    }
    // Vẽ các ô của mê cung
    for (int y = 0; y < maze.height; ++y)
    {
        for (int x = 0; x < maze.width; ++x)
        {
            Maze::Cell& cell = maze.grid[y][x];

            int left = x * cellSize;
            int top = y * cellSize;
            int right = left + cellSize;
            int bottom = top + cellSize;

            // Vẽ các bức tường
            if (cell.walls[0])  // Top
                dc.MoveTo(left, top), dc.LineTo(right, top);
            if (cell.walls[1])  // Right
                dc.MoveTo(right, top), dc.LineTo(right, bottom);
            if (cell.walls[2])  // Bottom
                dc.MoveTo(right, bottom), dc.LineTo(left, bottom);
            if (cell.walls[3])  // Left
                dc.MoveTo(left, bottom), dc.LineTo(left, top);
        }
    }

    // Tô màu lối vào (ô đầu tiên)
    dc.FillSolidRect(0, 0, cellSize, cellSize, RGB(255,255,255));  

    // Tô màu lối ra (ô cuối cùng)
    dc.FillSolidRect((maze.width - 1) * cellSize, (maze.height - 1) * cellSize, cellSize, cellSize, RGB(255, 0, 0));  // Màu đỏ

    
    // Vẽ người chơi
    int playerLeft = maze.playerX * cellSize ;
    int playerTop = maze.playerY * cellSize ;
    dc.FillSolidRect(playerLeft, playerTop, cellSize, cellSize, RGB(0, 0, 255));  // Màu xanh dương

    // Kiểm tra xem người chơi có thắng không
    if (maze.playerX == maze.width - 1 && maze.playerY == maze.height - 1)
    {
        dc.SetTextColor(RGB(0, 255, 0));  // Màu xanh lá
        dc.TextOut(maze.width * cellSize / 3, maze.height * cellSize / 2, _T("You Win!"));
    }
    // Kiểm tra xem người chơi có thua không (ví dụ: nếu người chơi không thể di chuyển)
    if (timerID==0) {
        dc.SetTextColor(RGB(255, 0, 0));  // Màu đỏ
        dc.TextOut(maze.width * cellSize / 3, maze.height * cellSize / 2, _T("You Lose!"));
    }
    restartButton.draw(dc);
    // Hiển thị thời gian còn lại
    int x = clientRect.Width() - 800;
    int y = 70;
    CString timeText;
    timeText.Format(_T("Time left: %d seconds"), remainingTime);
    dc.SetTextColor(RGB(255, 0, 0)); // Màu đỏ
    dc.TextOut(x, y, timeText);    // Hiển thị ở góc trên
    // Do not call CWnd::OnPaint() for painting messages
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
   

    // Di chuyển người chơi
    switch (nChar)
    {
    case VK_UP:
        maze.movePlayer(0, -1);  // Di chuyển lên
        break;
    case VK_DOWN:
        maze.movePlayer(0, 1);  // Di chuyển xuống
        break;
    case VK_LEFT:
        maze.movePlayer(-1, 0);  // Di chuyển sang trái
        break;
    case VK_RIGHT:
        maze.movePlayer(1, 0);  // Di chuyển sang phải
        break;
    }

    // Cập nhật màn hình sau khi di chuyển
    Invalidate();  // Vẽ lại màn hình
    CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}



void CChildView::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);
    
    // TODO: Add your message handler code here
}
void CChildView::OnLButtonDown(UINT nFlags, CPoint point) {
    if (restartButton.isClicked(point)) {
        if (timerID != 0)
            KillTimer(timerID); // Dừng bộ đếm hiện tại
        remainingTime = 900;     // Đặt lại thời gian
        timerID = SetTimer(1, 1000, nullptr); // Khởi động bộ đếm mới
        maze.resetMaze();
        maze.resetPlayerPosition();
        Invalidate();
    }

    CWnd::OnLButtonDown(nFlags, point);
}


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    TRACE(_T("Timer triggered: remainingTime = %d\n"), remainingTime);
    if (nIDEvent == timerID) {
        remainingTime--;
        if (remainingTime <= 0) {
            KillTimer(timerID);
            timerID = 0; // Đặt lại ID để tránh sử dụng sau khi dừng
          +  MessageBox(_T("Time's up! You Lose!"), _T("Game Over"), MB_OK | MB_ICONEXCLAMATION);
        }
        Invalidate();
    }
    CWnd::OnTimer(nIDEvent);
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;
    // Thiết lập bộ đếm thời gian
    remainingTime = 900; // Đặt thời gian ban đầu
    timerID = SetTimer(1, 1000, nullptr); // Bắt đầu timer (1000ms = 1s)
    if (timerID == 0) {
        MessageBox(_T("Failed to create timer!"), _T("Error"), MB_OK | MB_ICONERROR);
    }
    // TODO:  Add your specialized creation code here

    return 0;
}
