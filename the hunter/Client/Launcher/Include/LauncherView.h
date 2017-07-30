
// LauncherView.h : CLauncherView 클래스의 인터페이스
//

#pragma once

#include "LauncherDoc.h"

class CLauncherView : public CView
{
protected: // serialization에서만 만들어집니다.
	CLauncherView();
	DECLARE_DYNCREATE(CLauncherView)

// 특성입니다.
public:
	CLauncherDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 구현입니다.
public:
	virtual ~CLauncherView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // LauncherView.cpp의 디버그 버전
inline CLauncherDoc* CLauncherView::GetDocument() const
   { return reinterpret_cast<CLauncherDoc*>(m_pDocument); }
#endif

