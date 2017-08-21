
// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "Launcher.h"

#include "MainFrm.h"
#include "NetworkManager.h"
#include "LauncherValue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define LOGINSERVER_IP		"10.30.1.1"
#define LOGINSERVERPORT	9000

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
END_MESSAGE_MAP()

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	// â�� �������ش�.
	m_Splitter.CreateStatic(this, 1, 2);
	m_Splitter.CreateView(0, 0, RUNTIME_CLASS(CLauncherView), CSize(0, 400), pContext);
	m_Splitter.CreateView(0, 1, RUNTIME_CLASS(CLoginForm), CSize(600, 400), pContext);

	m_pView = (CLauncherView*)m_Splitter.GetPane(0, 0);
	m_pForm = (CLoginForm*)m_Splitter.GetPane(0, 1);

	// �α��� ������ ������ �Ѵ�.
//	if (!CNetworkManager::GetInst()->Init(LOGINSERVER_IP, LOGINSERVERPORT, this))
//		return FALSE;

	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;

	cs.style ^= FWS_ADDTOTITLE;

	cs.cx = 500;
	cs.cy = 400;

	cs.hMenu = 0;

	return TRUE;
}

// CMainFrame ����

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame �޽��� ó����

CLauncherView * CMainFrame::GetView() const
{
	return m_pView;
}
CLoginForm * CMainFrame::GetLoginForm() const
{
	return m_pForm;
}