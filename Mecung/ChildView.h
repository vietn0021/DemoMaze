
// ChildView.h : interface of the CChildView class
//


#pragma once
#include "Maze.h"
#include "Restart.h"


// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
private:
	Maze maze; // Đối tượng Maze để lưu trữ mê cung
	int cellSize; // Kích thước mỗi ô trong mê cung
	Restart restartButton; // Nút Restart
	UINT_PTR timerID;       // ID của bộ đếm thời gian
	int remainingTime=60;      // Thời gian còn lại (tính bằng giây)

public:

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point); // Bắt sự kiện chuột
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

