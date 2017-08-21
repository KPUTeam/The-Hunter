
// MainFrm.h : CMainFrame Ŭ������ �������̽�
//

#pragma once

#include "LauncherView.h"
#include "LoginForm.h"

class CMainFrame : public CFrameWnd
{
	
protected: // serialization������ ��������ϴ�.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Ư���Դϴ�.
protected:
	CSplitterWnd m_wndSplitter;
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// �����Դϴ�.
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


// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()

};


