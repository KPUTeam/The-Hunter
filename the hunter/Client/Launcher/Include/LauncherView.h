
// LauncherView.h : CLauncherView Ŭ������ �������̽�
//

#pragma once

#include "LauncherDoc.h"

class CLauncherView : public CView
{
protected: // serialization������ ��������ϴ�.
	CLauncherView();
	DECLARE_DYNCREATE(CLauncherView)

// Ư���Դϴ�.
public:
	CLauncherDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// �����Դϴ�.
public:
	virtual ~CLauncherView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // LauncherView.cpp�� ����� ����
inline CLauncherDoc* CLauncherView::GetDocument() const
   { return reinterpret_cast<CLauncherDoc*>(m_pDocument); }
#endif

