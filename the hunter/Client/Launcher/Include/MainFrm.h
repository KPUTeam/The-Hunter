
// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once

#include "LauncherView.h"
#include "LoginForm.h"

class CMainFrame : public CFrameWnd
{
	
protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 특성입니다.
protected:
	CSplitterWnd m_wndSplitter;
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	CSplitterWnd	m_Splitter;
	CLauncherView*	m_pView;
	CLoginForm*		m_pForm;
	

public:
	CLauncherView*	GetView() const;
	CLoginForm*	GetLoginForm() const;


// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

};


