
// LauncherView.cpp : CLauncherView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
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

// CLauncherView 생성/소멸

CLauncherView::CLauncherView()
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CLauncherView::~CLauncherView()
{
}

BOOL CLauncherView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CLauncherView 그리기

void CLauncherView::OnDraw(CDC* /*pDC*/)
{
	CLauncherDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CLauncherView 진단

#ifdef _DEBUG
void CLauncherView::AssertValid() const
{
	CView::AssertValid();
}

void CLauncherView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLauncherDoc* CLauncherView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLauncherDoc)));
	return (CLauncherDoc*)m_pDocument;
}
#endif //_DEBUG


// CLauncherView 메시지 처리기


void CLauncherView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}
