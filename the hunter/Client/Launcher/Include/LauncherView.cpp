
// LauncherView.cpp : CLauncherView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "Launcher.h"
#endif

#include "LauncherDoc.h"
#include "LauncherView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLauncherView

IMPLEMENT_DYNCREATE(CLauncherView, CView)

BEGIN_MESSAGE_MAP(CLauncherView, CView)
END_MESSAGE_MAP()

// CLauncherView ����/�Ҹ�

CLauncherView::CLauncherView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CLauncherView::~CLauncherView()
{
}

BOOL CLauncherView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CLauncherView �׸���

void CLauncherView::OnDraw(CDC* /*pDC*/)
{
	CLauncherDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CLauncherView ����

#ifdef _DEBUG
void CLauncherView::AssertValid() const
{
	CView::AssertValid();
}

void CLauncherView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLauncherDoc* CLauncherView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLauncherDoc)));
	return (CLauncherDoc*)m_pDocument;
}
#endif //_DEBUG


// CLauncherView �޽��� ó����


void CLauncherView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}
